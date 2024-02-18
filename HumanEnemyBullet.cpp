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
#include "CircleSkillShoot.h"
#include "audio.h"

using namespace DirectX::SimpleMath;

void Enemy::Shot::Init()
{
	// m_ScheduledTaskの初期化
	m_ScheduledTask = std::make_unique<Timer::ScheduledTask>(0.2f);
	m_SpellBulletRate = std::make_unique<Timer::ScheduledTask>(0.7f);
	//誘導弾モードに固定
	enemyShootModo = ENEMYSHOOTMODO::HOMING;
}

void Enemy::Shot::Update()
{
	//現在のシーンを取得
	Scene* scene = Manager::GetScene();
	//現在のシーンのプレイヤーのオブジェクトを取得
	Player::PlayerObject* player = scene->GetGameObject<Player::PlayerObject>();
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
			// 敵の現在位置に敵の前方ベクトルを加えて、ちょっと前にオフセットした位置を計算
			Vector3 enemySpawnShot = enemy->GetPosition() + forward * AddForwardEnemyShotSpawnPos;
			enemySpawnShot.y += 1.6f;
			// 敵の前方に向かってプレイヤーの位置を取得
			Vector3 directionToEnemy = player->GetPosition() - enemySpawnShot;
			directionToEnemy.Normalize();

			// 弾を作成し、エネミーの方向に速度を設定
			HomingBullet* bullet = scene->AddGameObject<HomingBullet>(2);
			//球は誰が打っているか
			bullet->SetBulletOwner(CHARACTER::ENEMY);
			bullet->SetPosition(enemySpawnShot);
			bullet->SetVelocity(directionToEnemy * 0.5f);
			addShotFlg = false;
		}

		if (m_SpellBulletRate->GetFlg())
		{
			// プレイヤーの現在位置にプレイヤーの前方ベクトルを加えて、ちょっと前にオフセットした位置を計算
			Vector3 enemySpawnShot = enemy->GetPosition();

			// 弾を作成し、エネミーの方向に速度を設定
			std::array<CircleSkillShoot*, 5> bullets;
			int Defdegree = 3;
			float slidePosx = 5.0f;
			for (auto i = 0; i < bullets.size(); i++)
			{
				Vector3 newEnemySpawnShot = enemySpawnShot;

				newEnemySpawnShot.x + slidePosx * -2 + slidePosx * i;

				bullets.at(i) = scene->AddGameObject<CircleSkillShoot>(2);
				bullets.at(i)->SetBulletOwner(CHARACTER::ENEMY);
				bullets.at(i)->SetPosition(enemySpawnShot);
				bullets.at(i)->SetBulletDegree(Defdegree * -2 + Defdegree * i);
			}
		}

		std::vector<HumanObject*> enemyList = scene->GetGameObjects<HumanObject>();
		std::vector<HomingBullet*> bulletList = scene->GetGameObjects<HomingBullet>();

		//球への当たり判定
		for (HomingBullet* bullet : bulletList)
		{
			Vector3 playerPosition = player->GetPosition();
			BoundingSphereObj* playerHitSphere = player->GetPlayerHitSphere();
			BoundingSphereObj* bulletHitSphere = bullet->GetBulletHitSphere();

			if (bullet->GetBulletOwner() == CHARACTER::ENEMY)
			{
				//球の当たり判定
				if (IsCollision(*playerHitSphere, *bulletHitSphere))
				{
					Score* score = scene->GetGameObject<Score>();
					Sound::Audio* m_SE = player->GetComponent <Sound::Audio >();
					m_SE->Play();
					player->Damege(1);
					bullet->SetDestroy();
				}
			}
		}

		//CircleSkillShootの当たり判定

		std::vector<CircleSkillShoot*> skillBulletList = scene->GetGameObjects<CircleSkillShoot>();

		//球への当たり判定
		for (CircleSkillShoot* bullet : skillBulletList)
		{
			Vector3 playerPosition = player->GetPosition();
			BoundingSphereObj* playerHitSphere = player->GetPlayerHitSphere();
			BoundingSphereObj* bulletHitSphere = bullet->GetBulletHitSphere();

			//オーナーがプレイヤーなら当たり判定
			if (bullet->GetBulletOwner() == CHARACTER::ENEMY)
			{
				//球の当たり判定
				if (IsCollision(*playerHitSphere, *bulletHitSphere))
				{
					Score* score = scene->GetGameObject<Score>();
					Sound::Audio* m_SE = enemy->GetComponent <Sound::Audio >();
					m_SE->Play();
					player->Damege(3);
					bullet->SetDestroy();
				}
			}
		}
	}
}

void Enemy::Shot::AddShot()
{
	addShotFlg = true;
}