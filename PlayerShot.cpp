#include "PlayerShot.h"
#include "input.h"
#include "player.h"
#include "scene.h"
#include "manager.h"
#include "camera.h"
#include "HomingBullet.h"

#include "CollisionUtils.h"
#include "HumanEnemy.h"
#include "enemy.h"
#include "score.h"

using namespace DirectX::SimpleMath;
using namespace Player;
using namespace Timer;
using namespace Enemy;

void Player::Shot::Init()
{
	// m_ScheduledTaskの初期化
	m_ScheduledTask = std::make_unique<ScheduledTask>(0.3f);
}

void Player::Shot::Update()
{
	//現在のシーンを取得
	Scene* scene = Manager::GetScene();
	//現在のシーンのプレイヤーのオブジェクトを取得
	PlayerObject* player = scene->GetGameObject<PlayerObject>();
	//現在のシーンのカメラを取得
	Camera* cameraobj = scene->GetGameObject<Camera>();

	Matrix viewmtx = cameraobj->GetViewMatrix();
	Vector3 ZAxis = Vector3(viewmtx._13, 0.0f, viewmtx._33);

	//前向きベクトルを取得
	Vector3 forward = ZAxis;

	//弾発射
	if (Input::GetKeyPress(VK_LBUTTON))
	{
		if (m_ScheduledTask->GetFlg())
		{
			HomingBullet* bullet = scene->AddGameObject<HomingBullet>(2);
			bullet->SetPosition(player->GetPosition() + Vector3(0.0f, 1.0f, 0.0f));
			bullet->SetVelocity(forward * 0.5f);
			addShotFlg = false;
		}
	}

	HumanObject* enemy = scene->GetGameObject<HumanObject>();

	std::vector<HumanObject*> enemyList = scene->GetGameObjects<HumanObject>();
	std::vector<HomingBullet*> bulletList = scene->GetGameObjects<HomingBullet>();

	//敵への当たり判定
	for (HumanObject* enemy : enemyList)
	{
		//球への当たり判定
		for (HomingBullet* bullet : bulletList)
		{
			Vector3 enemyPosition = enemy->GetPosition();
			BoundingSphere* enemyHitSphere = enemy->GetEnemyHitSphere();
			BoundingSphere* bulletHitSphere = bullet->GetBulletHitSphere();

			//球の当たり判定
			if (IsCollision(*enemyHitSphere, *bulletHitSphere))
			{
				Score* score = scene->GetGameObject<Score>();
				score->AddCount(1);

				bullet->SetDestroy();
			}
		}
	}
}

void Player::Shot::AddShot()
{
	addShotFlg = true;
}