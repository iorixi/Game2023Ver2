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
#include "PlayerShootModo.h"
#include "CircleSkillShoot.h"
#include <array>

using namespace DirectX::SimpleMath;

void Player::Shot::Init()
{
	// m_ScheduledTaskの初期化
	m_ScheduledTask = std::make_unique<Timer::ScheduledTask>(0.15f);
	m_ShootSkill = std::make_unique<Timer::ScheduledTask>();
	m_ShootSkillCool = std::make_unique<Timer::ScheduledTask>();
	m_ShootSkillRate = std::make_unique<Timer::ScheduledTask>();

	//誘導弾モードに固定
	playerShootModo = PlayerShootModo::HOMING;
}

void Player::Shot::Update()
{
	//現在のシーンを取得
	Scene* scene = Manager::GetScene();
	//現在のシーンのプレイヤーのオブジェクトを取得
	Player::PlayerObject* player = scene->GetGameObject<Player::PlayerObject>();
	//現在のシーンの敵のオブジェクトを取得
	Enemy::HumanObject* enemy = scene->GetGameObject<Enemy::HumanObject>();

	//現在のシーンのカメラを取得
	Camera* cameraobj = scene->GetGameObject<Camera>();

	Matrix viewmtx = cameraobj->GetViewMatrix();
	Vector3 ZAxis = Vector3(viewmtx._13, 0.0f, viewmtx._33);

	//前向きベクトルを取得
	Vector3 forward = ZAxis;

	//アクティブ状態なら
	if (player->GetIsActive())
	{
		//スペル１発動
		if (Input::GetKeyTrigger('Q' || VK_LBUTTON || VK_UP))
		{
			if (m_ShootFlg)
			{
				player->SetActionModo(ActionModo::SPELL1);
				m_ShootSkill->SetTimer(0.5f);
				m_ShootFlg = false;

				// プレイヤーの現在位置にプレイヤーの前方ベクトルを加えて、ちょっと前にオフセットした位置を計算
				Vector3 playerSpawnShot = player->GetPosition();

				// 弾を作成し、エネミーの方向に速度を設定
				std::array<CircleSkillShoot*, 5> bullets;
				int Defdegree = 3;
				float slidePosx = 5.0f;
				for (auto i = 0; i < bullets.size(); i++)
				{
					Vector3 newPlayerSpawnShot = playerSpawnShot;

					newPlayerSpawnShot.x + slidePosx * -2 + slidePosx * i;

					bullets.at(i) = scene->AddGameObject<CircleSkillShoot>(2);
					bullets.at(i)->SetBulletOwner(CHARACTER::PLAYER);
					bullets.at(i)->SetPosition(playerSpawnShot);
					bullets.at(i)->SetBulletDegree(Defdegree * -2 + Defdegree * i);
				}
			}
		}

		if (m_ShootSkill->GetFlg())
		{
			player->SetActionModo(ActionModo::NONE);
			m_ShootFlg = true;
		}

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
	}
}

void Player::Shot::AddShot()
{
	addShotFlg = true;
}