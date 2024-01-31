#include "HomingBullet.h"
#include "main.h"
#include "scene.h"
#include "manager.h"
#include "modelRenderer.h"
#include "enemy.h"
#include "shadow.h"
#include "shader.h"
#include "explosion.h"
#include "score.h"
#include "BoundingSphere.h"

#include "HumanEnemy.h"
#include "player.h"
#include "CharaEnum.h"
#include "camera.h"
#include "ScheduledTask.h"
#include "ToRadians.h"

using namespace DirectX::SimpleMath;
using namespace Player;
using namespace Enemy;
using namespace Timer;

HomingBullet::HomingBullet()
{
}

HomingBullet::~HomingBullet()
{
}

void HomingBullet::Load()
{
	ModelRenderer::Preload("asset\\model\\bullet.obj");
}

void HomingBullet::Unload()
{
}

void HomingBullet::Init()
{
	AddComponent<Shader>()->Load("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");
	AddComponent<ModelRenderer>()->Load("asset\\model\\bullet.obj");

	AddComponent<Shadow>()->SetSize(0.5f);

	//子オブジェクトに当たり判定を追加
	BoundingSphere* boundingSphere = new BoundingSphere(1, m_Position);
	m_Child = AddChild<BoundingSphere>();
	m_Child = boundingSphere;

	//時間関係初期化
	m_HomingPointUpdateTime = std::make_shared<ScheduledTask>(homingPointUpdateTime);
}

void HomingBullet::Update()
{
	// 現在のシーンを取得
	Scene* currentScene = Manager::GetScene();
	// 現在のシーンのプレイヤーのオブジェクトを取得
	PlayerObject* player = currentScene->GetGameObject<PlayerObject>();
	// 現在のシーンの敵のオブジェクトを取得
	HumanObject* enemy = currentScene->GetGameObject<HumanObject>();

	// プレイヤーの座標を取得
	Vector3 playerPosition = player->GetPosition();
	Vector3 enemyPosition = enemy->GetPosition();

	//一定時間経つまで処理
	if (m_HomingPointUpdateTime->GetFlg())
	{
		// 球の移動方向を更新
		// 敵に向かうベクトルを計算
		directionToEnemy = enemyPosition - m_Position;
		directionToEnemy.Normalize();

		// 前のフレームのベクトルとの角度を計算
		float angleChange = oldDirection.Dot(directionToEnemy);
		// ベクトルの角度が45度以上変わった場合は無効にする
		if (angleChange <= cos(ToRadians(overAngle)))
		{
			isActive = false;
		}
		else
		{
			isActive = true;
		}

		oldDirection = directionToEnemy;
	}

	// 球の追尾が有効の場合のみ処理を行う
	if (isActive)
	{
		// 敵と球の距離を計算
		float distanceToEnemy = Vector3::Distance(enemyPosition, m_Position);

		// 敵との距離が一定以下の場合は追尾する
		if (distanceToEnemy <= closeDistance)
		{
			// 方向にスピードをかける
			m_Velocity = directionToEnemy * speed;

			// 球の位置を更新
			m_Position += m_Velocity;
		}
		else
		{
			// ホーミング弾が一定距離以上の敵に接近した場合、追尾を無効にして直進する
			isActive = false;
			Vector3 forward = m_Velocity;
			forward.Normalize();

			// ホーミング弾が直進する方向を設定する
			m_Velocity = forward * speed; // forwardはホーミング弾の前方向ベクトルを表すものとします

			// 球の位置を更新
			m_Position += m_Velocity;
		}
	}
	else
	{
		// ホーミング弾が一定距離以上の敵に接近した場合、追尾を無効にして直進する
		isActive = false;
		Vector3 forward = m_Velocity;
		forward.Normalize();
		// ホーミング弾が直進する方向を設定する
		m_Velocity = forward * speed; // forwardはホーミング弾の前方向ベクトルを表すものとします

		// 球の位置を更新
		m_Position += m_Velocity;
	}

	// 球の位置が一定範囲を超えた場合は削除
	if (m_Position.Length() > 100.0f)
	{
		SetDestroy();
	}

	// 当たり判定の位置を更新
	m_Child->SetCenter(m_Position);
}