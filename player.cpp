#include "manager.h"
#include "renderer.h"
#include "shader.h"
#include "Player.h"
#include "input.h"
#include "box.h"
#include "audio.h"
#include "bullet.h"
#include "shadow.h"
#include "camera.h"
#include "field.h"
#include "PlayerMove.h"
#include "PlayerFloating.h"
#include "PlayerEvasive.h"
#include "PlayerShot.h"
#include "HumanEnemy.h"
#include "BoundingSphere.h"
#include "ActionModo.h"
#include "score.h"
#include "PlayerBullerHit.h"

#include <vector>

#include "newModel/CBoundingBox.h"
#include "newModel/CShader.h"
#include "newModel/CStaticMesh.h"
#include "newModel/CStaticMeshRenderer.h"
#include "newModel/CSphereMesh.h"
#include "newModel/dx11mathutil.h"

using namespace DirectX::SimpleMath;

// プレイヤーの移動速度
const float moveSpeed = 1.5f;

// 描画の為に必要な情報
// 使用するシェーダー
static CShader g_shader;
// スタティックメッシュ（ジオメトリデータ）
static CStaticMesh g_staticmesh;
// メッシュレンダラー
static CStaticMeshRenderer g_staticmeshrenderer;
// 球
static CSphereMesh g_sphere;// メッシュレンダラ
static CMeshRenderer g_meshrenderer;
// OBB
static CBoundingBox g_obb;

// プレイヤーオブジェクトの初期化
void Player::PlayerObject::Init()
{
	// シェーダーコンポーネントを追加してシェーダーをロードする
	AddComponent<Shader>()->Load("shader\\vertexLightingOneSkinVS.cso", "shader\\vertexLightingPS.cso");

	// モデルの初期化
	{
		// 使用するシェーダーを生成
		g_shader.SetShader(
			"shader/vertexLightingVS.hlsl",   // 頂点シェーダ
			"shader/vertexLightingPS.hlsl");  // ピクセルシェーダ

		// モデルファイル名
		std::string filename[] = {
			"asset\\model\\youmu\\youmu2.pmx",
			"asset\\model\\Akai_Run.fbx",
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

	// 音声用のコンポーネントを追加してロードする
	m_SE = AddComponent<Sound::Audio>();
	m_SE->Load("asset\\audio\\damage2.wav");

	m_Scale = Vector3(0.15f, 0.15f, 0.15f);

	// タイマー用の遅延コンポーネントを追加
	m_Delay = AddComponent<Timer::DelayCompnent>();
	m_Delay->SetLoop(true);
	m_Delay->SetdelayTime(0.2f);

	// プレイヤーコンポーネントを追加
	m_PlayerMove = AddComponent<Player::Move>();
	m_PlayerFloating = AddComponent<Player::Floating>();
	m_PlayerEvasive = AddComponent<Player::Evasive>();
	m_PlayerShot = AddComponent<Player::Shot>();
	m_BulletHit = AddComponent<Player::BulletHit>();

	m_Position.y += 10;
	m_Position.x += 40;

	// 子オブジェクトに当たり判定を追加
	playerHitSphere = std::make_shared<BoundingSphereObj>(0.2f, m_Position);
	actionModo = ActionModo::NONE;

	m_BGM = AddComponent<Sound::Audio>();
	m_BGM->Load("asset\\audio\\Dragoon_Rance.wav");
	m_BGM->Play();
}

// プレイヤーオブジェクトの更新
void Player::PlayerObject::Update()
{
	Vector3 oldPosition = m_Position;

	Scene* nowscene = Manager::GetScene();
	Camera* cameraobj = nowscene->GetGameObject<Camera>();

	Matrix viewmtx = cameraobj->GetViewMatrix();
	Vector3 ZAxis = Vector3(viewmtx._13, 0.0f, viewmtx._33);
	Vector3 XAxis = Vector3(viewmtx._11, 0.0f, viewmtx._31);

	// 前向きベクトルを取得
	Vector3 forward = ZAxis;

	actionModo = ActionModo::NONE;

	if (Input::GetKeyPress('W'))
	{
		m_BlendRate += 0.1f;
		m_Frame++;
	}
	else if (Input::GetKeyPress('S'))
	{
		m_BlendRate += 0.1f;
		m_Frame--;
	}
	else
	{
		m_BlendRate -= 2.0f;
		m_Frame++;
	}

	Enemy::HumanObject* enemyObject = nowscene->GetGameObject<Enemy::HumanObject>();

	// 敵オブジェクトの位置を取得
	Vector3 enemyPosition = enemyObject->GetPosition();
	Vector3 toEnemy;
	toEnemy.x = enemyPosition.x - m_Position.x;
	toEnemy.y = enemyPosition.y - m_Position.y;
	toEnemy.z = enemyPosition.z - m_Position.z;

	// プレイヤーオブジェクトから敵オブジェクトへの方向ベクトルを計算
	toEnemy.Normalize();
	// Y軸回りの回転角度を計算
	float yaw = atan2f(toEnemy.x, toEnemy.z);
	// X軸回りの回転角度を計算
	float pitch = -atan2f(toEnemy.y, sqrtf(toEnemy.x * toEnemy.x + toEnemy.z * toEnemy.z));
	// 回転を適用
	m_Rotation = Vector3(pitch, yaw, m_Rotation.z);

	// 抵抗
	m_Velocity.y -= m_Velocity.y * 0.01f;

	// 移動
	m_Position += m_Velocity * moveSpeed;

	// フィールドオブジェクト取得
	Field* fieldobj = nowscene->GetGameObject<Field>();

	// 範囲チェック
	Vector3 max = fieldobj->GetMax();
	Vector3 min = fieldobj->GetMin();

	if (m_Position.x <= min.x) {
		m_Position.x = min.x;
	}
	if (m_Position.x >= max.x) {
		m_Position.x = max.x;
	}

	if (m_Position.z <= min.z) {
		m_Position.z = min.z;
	}
	if (m_Position.z >= max.z) {
		m_Position.z = max.z;
	}

	// 接地
	float groundHeight = fieldobj->GetFieldHeightBySqno(m_Position);

	// 位置が０以下なら地面位置にセットする
	if (m_Position.y < groundHeight && m_Velocity.y < 0.0f)
	{
		m_Position.y = groundHeight;
		m_Velocity.y = 0.0f;
	}

	// 現在のシーンのプレイヤーのオブジェクトを取得
	Player::PlayerObject* player = nowscene->GetGameObject<Player::PlayerObject>();
	Evasive* playerEvasive = player->GetComponent<Evasive>();

	if (m_BlendRate > 1.0f)
	{
		m_BlendRate = 1.0f;
	}
	if (m_BlendRate < 0.0f)
	{
		m_BlendRate = 0.0f;
	}

	// 当たり判定
	playerHitSphere->SetCenter(m_Position);
	std::vector<Score*> score = nowscene->GetGameObjects<Score>();
	score.at(1)->SetCount(hp);
}

// プレイヤーオブジェクトの描画前処理
void Player::PlayerObject::PreDraw()
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

// プレイヤーオブジェクトの終了処理
void Player::PlayerObject::Uninit()
{
	GameObject::Uninit();
	g_staticmesh.Exit();
}

// アクティブ状態を設定する
void Player::PlayerObject::SetIsActive(bool _isActive)
{
	isActive = _isActive;
}

// アクティブ状態を取得する
bool Player::PlayerObject::GetIsActive()
{
	return isActive;
}

// ダメージを受ける
void Player::PlayerObject::Damege(int damege)
{
	if (hp < damege)
	{
		hp = 0;
	}
	else
	{
		hp -= damege;
	}
}

// HPを取得する
int Player::PlayerObject::GetHp()
{
	return hp;
}

// アクションモードを取得する
ActionModo Player::PlayerObject::GetActionModo()
{
	return actionModo;
}

// アクションモードを設定する
void Player::PlayerObject::SetActionModo(ActionModo actionmodo)
{
	actionModo = actionmodo;
}