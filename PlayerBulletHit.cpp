#include "PlayerBullerHit.h"
#include <vector>
#include "scene.h"
#include "HumanEnemy.h"
#include "HomingBullet.h"
#include "manager.h"
#include "renderer.h"
#include "CharaEnum.h"
#include "collision.h"
#include "cylinder.h"
#include "CollisionUtils.h"

using namespace DirectX::SimpleMath;

void Player::BulletHit::Update()
{
	//球の当たり判定処理
	HomingHit();
}

void Player::BulletHit::HomingHit()
{
	hitFlg = false;
	//現在のシーンを取得
	Scene* scene = Manager::GetScene();

	//現在のシーンの敵のオブジェクトを取得
	Enemy::HumanObject* enemy = scene->GetGameObject<Enemy::HumanObject>();

	std::vector<Enemy::HumanObject*> enemyList = scene->GetGameObjects<Enemy::HumanObject>();
	std::vector<HomingBullet*> bulletList = scene->GetGameObjects<HomingBullet>();

	//敵への当たり判定
	for (Enemy::HumanObject* enemy : enemyList)
	{
		//球への当たり判定
		for (HomingBullet* bullet : bulletList)
		{
			Vector3 enemyPosition = enemy->GetPosition();
			BoundingSphereObj* enemyHitSphere = enemy->GetEnemyHitSphere();
			BoundingSphereObj* bulletHitSphere = bullet->GetBulletHitSphere();

			//オーナーがプレイヤーなら当たり判定
			if (bullet->GetBulletOwner() == CHARACTER::PLAYER)
			{
				//球の当たり判定
				if (IsCollision(*enemyHitSphere, *bulletHitSphere))
				{
					//score->AddCount(1);
					//Sound::Audio* m_SE = player->GetComponent<Sound::Audio>();
					//m_SE->Play();
					hitFlg = true;

					enemy->Damege(1);
					bullet->SetDestroy();
				}
			}
		}
	}
}

bool Player::BulletHit::GetHitFlg()
{
	return hitFlg;
}