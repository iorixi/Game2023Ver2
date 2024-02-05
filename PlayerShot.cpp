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
#include "CharaEnum.h"
#include "BoundingSphere.h"
#include "ActionModo.h"

#include "audio.h"

using namespace DirectX::SimpleMath;
using namespace Player;
using namespace Timer;
using namespace Enemy;
using namespace Sound;

void Player::Shot::Init()
{
	// m_ScheduledTaskの初期化
	m_ScheduledTask = std::make_unique<ScheduledTask>(0.15f);
	//誘導弾モードに固定
	PlayerShootModo = PLAYERSHOOTMODO::HOMING;
}

void Player::Shot::Update()
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
	if (player->GetIsActive())
	{
		//移動状態もしくは何もしてないときは球を発射する
		if (player->GetActionModo() == ActionModo::NONE || player->GetActionModo() == ActionModo::MOVE)
		{
			//玉を生成
			if (m_ScheduledTask->GetFlg())
			{
				// プレイヤーの現在位置にプレイヤーの前方ベクトルを加えて、ちょっと前にオフセットした位置を計算
				Vector3 playerSpawnShot = player->GetPosition() + forward * AddForwardPlayerShotSpawnPos;
				playerSpawnShot.y += 1.8f;

				// プレイヤーの前方に向かってエネミーの位置を取得
				Vector3 directionToEnemy = enemy->GetPosition() - playerSpawnShot;
				directionToEnemy.Normalize();

				// 弾を作成し、エネミーの方向に速度を設定
				HomingBullet* bullet = scene->AddGameObject<HomingBullet>(2);
				//球は誰が打っているか
				bullet->SetBulletOwner(CHARACTER::PLAYER);
				bullet->SetPosition(playerSpawnShot);
				bullet->SetVelocity(directionToEnemy * 0.5f);
				addShotFlg = false;
			}
		}

		std::vector<HumanObject*> enemyList = scene->GetGameObjects<HumanObject>();
		std::vector<HomingBullet*> bulletList = scene->GetGameObjects<HomingBullet>();

		//敵への当たり判定
		for (HumanObject* enemy : enemyList)
		{
			//球への当たり判定
			for (HomingBullet* bullet : bulletList)
			{
				Vector3 enemyPosition = enemy->GetPosition();
				BoundingSphereObj* enemyHitSphere = enemy->GetEnemyHitSphere();
				BoundingSphereObj* bulletHitSphere = bullet->GetBulletHitSphere();

				if (bullet->GetBulletOwner() == CHARACTER::PLAYER)
				{
					//球の当たり判定
					if (IsCollision(*enemyHitSphere, *bulletHitSphere))
					{
						//score->AddCount(1);
						Sound::Audio* m_SE = player->GetComponent<Audio>();
						m_SE->Play();

						enemy->Damege(1);
						bullet->SetDestroy();
					}
				}
			}
		}
	}
}

void Player::Shot::AddShot()
{
	addShotFlg = true;
}