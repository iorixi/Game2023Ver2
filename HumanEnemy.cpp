#include "manager.h"
#include "input.h"
#include "scene.h"
#include "HumanEnemy.h"
#include "shader.h"
#include "bullet.h"
#include "player.h"
#include "HomingBullet.h"
#include "CharaEnum.h"
#include "HumanEnemyBullet.h"
#include "BoundingSphere.h"
#include "ScheduledTask.h"
#include "Random.h"
#include "ActionModo.h"
#include "score.h"
#include <vector>

#include "newModel/CBoundingBox.h"
#include "newModel/CShader.h"
#include "newModel/CStaticMesh.h"
#include "newModel/CStaticMeshRenderer.h"
#include "newModel/CSphereMesh.h"
#include "newModel/dx11mathutil.h"

// 使用するシェーダー
static CShader	g_shader;
// スタティックメッシュ（ジオメトリデータ）
static CStaticMesh g_staticmesh;
// メッシュレンダラー
static CStaticMeshRenderer g_staticmeshrenderer;
// 球
static CSphereMesh g_sphere;// メッシュレンダラ
static CMeshRenderer g_meshrenderer;
// OBB
static CBoundingBox g_obb;

using namespace DirectX::SimpleMath;

// 敵の人間オブジェクトの初期化
void Enemy::HumanObject::Init()
{
	AddComponent<Shader>()->Load("shader\\vertexLightingOneSkinVS.cso", "shader\\vertexLightingPS.cso"); // シェーダーのロード

	m_Position.y = 10; // Y座標の初期化

	// モデルの初期化
	{
		// 使用するシェーダーを生成
		g_shader.SetShader(
			"shader/vertexLightingVS.hlsl",   // 頂点シェーダ
			"shader/vertexLightingPS.hlsl");  // ピクセルシェーダ

		// モデルファイル名
		std::string filename[] = {
			"asset\\model\\furan\\furan2.pmx",
		};

		// メッシュ生成（ジオメトリデータ）
		g_staticmesh.Init(filename[0]);

		// 描画の為のデータ生成
		g_staticmeshrenderer.Init(g_staticmesh);

		// マテリアル生成
		MATERIAL mtrl;
		mtrl.Ambient = Color(0, 0, 0, 0);
		mtrl.Diffuse = Color(1, 1, 1, 0.3f);
		mtrl.Specular = Color(0, 0, 0, 0);
		mtrl.Shininess = 0;
		mtrl.Emission = Color(0, 0, 0, 0);
		mtrl.TextureEnable = false;

		g_material.Init(mtrl);
	}

	m_Scale = Vector3(0.15f, 0.15f, 0.15f); // スケールの初期化

	// 子オブジェクトに当たり判定を追加
	enemyHitSphere = std::make_shared<BoundingSphereObj>(0.1f, m_Position);

	// 移動切り替えタスクの初期化
	m_MoveChangeTask = std::make_unique<Timer::ScheduledTask>(4.0f);

	m_EnemyShot = AddComponent<Enemy::Shot>(); // 敵のショットコンポーネントの追加
}

// 敵の人間オブジェクトの終了処理
void Enemy::HumanObject::Uninit()
{
	GameObject::Uninit();
	g_staticmesh.Exit(); // メッシュの終了処理
}

// 敵の人間オブジェクトの更新
void Enemy::HumanObject::Update()
{
	Scene* nowscene = Manager::GetScene(); // 現在のシーンの取得

	m_MoveTime += 1.0f / 60.0f; // 移動時間の更新

	// 移動パターンの切り替え
	if (cosf(m_MoveTime * 1.0f) * 0.1f > 0)
	{
		m_BlendRate += 0.1f;
		m_Frame++;
	}
	else if (cosf(m_MoveTime * 1.0f) * 0.1f < 0)
	{
		m_BlendRate += 0.1f;
		m_Frame--;
	}
	else
	{
		m_BlendRate -= 0.1f;
		m_Frame++;
	}

	// プレイヤーオブジェクトの位置を取得
	Player::PlayerObject* playerObject = nowscene->GetGameObject<Player::PlayerObject>();
	Vector3 playerPos = playerObject->GetPosition();
	Vector3 toPlayer;
	toPlayer.x = playerPos.x - m_Position.x;
	toPlayer.y = playerPos.y - m_Position.y;
	toPlayer.z = playerPos.z - m_Position.z;

	// プレイヤーオブジェクトから敵オブジェクトへの方向ベクトルを計算
	toPlayer.Normalize();

	// Y軸回りの回転角度を計算
	float yaw = atan2f(toPlayer.x, toPlayer.z);
	// X軸回りの回転角度を計算
	float pitch = atan2f(toPlayer.y, sqrtf(toPlayer.x * toPlayer.x + toPlayer.z * toPlayer.z));

	// Z軸回りの回転角度（このサンプルでは固定で0.0f）
	float roll = 0.0f;

	// 移動パターンの選択
	if (m_MoveChangeTask->GetFlg())
	{
		C_Random random;
		actionModoSelect = random.Get_Random(1, 5);
		if (actionModoSelect == 5)
		{
			actionModoSelect = random.Get_Random(1, 5);
		}
	}

	// 移動パターンによる行動
	switch (actionModoSelect)
	{
	case 1:
		// 右移動
		m_Position.x += 0.2f; // 例: 0.1f単位で左に移動
		actionModo = ActionModo::MOVE;
		break;
	case 2:
		// 右移動
		m_Position.x += 0.2f; // 例: 0.1f単位で左に移動
		actionModo = ActionModo::MOVE;
		break;
	case 3:
		// 左移動
		m_Position.x -= 0.2f; // 例: 0.1f単位で左に移動
		actionModo = ActionModo::MOVE;
		break;
	case 4:
		// 左移動
		m_Position.x -= 0.2f; // 例: 0.1f単位で左に移動
		actionModo = ActionModo::MOVE;
		break;
	case 5:
		// 止まる
		actionModo = ActionModo::SPELL1;
		break;
	default:
		break;
	}

	// 回転を適用
	m_Rotation = Vector3(pitch, yaw, roll);

	// 当たり判定
	enemyHitSphere->SetCenter(m_Position);
	std::vector<Score*> score = nowscene->GetGameObjects<Score>();
	score.at(0)->SetCount(hp);
}

// 敵の人間オブジェクトの描画前処理
void Enemy::HumanObject::PreDraw()
{
	// マテリアル処理
	// デバイスコンテキスト取得
	ID3D11DeviceContext* devicecontext;
	devicecontext = Renderer::GetDeviceContext();

	// ワールド変換行列生成
	Matrix mtx;
	DX11MakeWorldMatrixRadian(
		mtx,
		m_Scale,                            // スケール
		m_Rotation,                         // 姿勢
		m_Position);                        // 位置

	// GPUに行列をセットする
	Renderer::SetWorldMatrix(&mtx);

	// シェーダーをGPUにセット
	g_shader.SetGPU();

	// モデル描画
	g_staticmeshrenderer.Draw();

	// 境界ボックス描画
	mtx = g_obb.MakeWorldMtx(m_Scale, mtx);

	Renderer::SetWorldMatrix(&mtx);
	g_material.SetGPU();
	g_meshrenderer.Draw();
}

// アクティブ状態を設定する
void Enemy::HumanObject::SetIsActive(bool _isActive)
{
	isActive = _isActive;
}

// アクティブ状態を取得する
bool Enemy::HumanObject::GetIsActive()
{
	return isActive;
}

// アクションモードを取得する
ActionModo Enemy::HumanObject::GetActionModo()
{
	return actionModo;
}

// アクションモードを設定する
void Enemy::HumanObject::SetActionModo(ActionModo actionmodo)
{
	actionModo = actionmodo;
}