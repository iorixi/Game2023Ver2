#include "HumanEnemyBullet.h"
#include "player.h"
#include "scene.h"
#include "manager.h"
#include "camera.h"
#include "HomingBullet.h"

#include "CollisionUtils.h"
#include "HumanEnemy.h"
#include "enemy.h"
#include "score.h"
#include "CharaEnum.h"
#include "ScheduledTask.h"
#include "BoundingSphere.h"

using namespace DirectX::SimpleMath;
using namespace Player;
using namespace Timer;
using namespace Enemy;

void Enemy::Shot::Init()
{
	// m_ScheduledTaskの初期化
	m_ScheduledTask = std::make_unique<ScheduledTask>(0.2f);
	//誘導弾モードに固定
	enemyShootModo = ENEMYSHOOTMODO::HOMING;
}

void Enemy::Shot::Update()
{
	//現在のシーンを取得
	Scene* scene = Manager::GetScene();
	//現在のシーンのプレイヤーのオブジェクトを取得
	PlayerObject* player = scene->GetGameObject<PlayerObject>();
	//現在のシーンの敵のオブジェクトを取得
	HumanObject* enemy = scene->GetGameObject<HumanObject>();

	//現在のシーンのカメラを取得
	Camera* cameraobj = scene->GetGameObject<Camera>();

	Matrix viewmtx = cameraobj->GetViewMatrix();
	Vector3 ZAxis = Vector3(viewmtx._13, 0.0f, viewmtx._33);

	//前向きベクトルを取得
	Vector3 forward = ZAxis;

	//アクティブ状態なら
	if (enemy->GetIsActive())
	{
		if (m_ScheduledTask->GetFlg())
		{
			// 敵のの現在位置に敵の前方ベクトルを加えて、ちょっと前にオフセットした位置を計算
			Vector3 enemySpawnShot = enemy->GetPosition() + forward * AddForwardEnemyShotSpawnPos;

			// 敵の前方に向かってプレイヤーの位置を取得
			Vector3 directionToEnemy = player->GetPosition() - enemySpawnShot;
			directionToEnemy.Normalize();

			// 弾を作成し、エネミーの方向に速度を設定
			HomingBullet* bullet = scene->AddGameObject<HomingBullet>(2);
			//球は誰が打っているか
			bullet->SetBulletOwner(CHARACTER::PLAYER);
			bullet->SetPosition(enemySpawnShot);
			bullet->SetVelocity(directionToEnemy * 0.5f);
			addShotFlg = false;
		}

		std::vector<HumanObject*> enemyList = scene->GetGameObjects<HumanObject>();
		std::vector<HomingBullet*> bulletList = scene->GetGameObjects<HomingBullet>();

		//球への当たり判定
		for (HomingBullet* bullet : bulletList)
		{
			Vector3 playerPosition = player->GetPosition();
			BoundingSphereObj* enemyHitSphere = player->GetPlayerHitSphere();
			BoundingSphereObj* bulletHitSphere = bullet->GetBulletHitSphere();

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

void Enemy::Shot::AddShot()
{
	addShotFlg = true;
}