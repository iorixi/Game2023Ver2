#include "CircleSkillShoot.h"
#include "scene.h"
#include "manager.h"
#include "modelRenderer.h"
#include "enemy.h"
#include "shadow.h"
#include "shader.h"
#include "score.h"
#include "BoundingSphere.h"
#include "HumanEnemy.h"
#include "player.h"
#include "CharaEnum.h"
#include "camera.h"
#include "ScheduledTask.h"
#include "CharaEnum.h"
#include <iostream>

using namespace DirectX::SimpleMath;

CircleSkillShoot::CircleSkillShoot()
{
}

CircleSkillShoot::CircleSkillShoot(CHARACTER chara)
{
	ownerChara = chara;
	if (ownerChara == CHARACTER::PLAYER)
	{
		// 現在のシーンを取得
		Scene* currentScene = Manager::GetScene();
		// 現在のシーンのプレイヤーのオブジェクトを取得
		Player::PlayerObject* player = currentScene->GetGameObject<Player::PlayerObject>();

		m_PositionStart = player->GetPosition();
	}
	else if (ownerChara == CHARACTER::ENEMY)
	{
		// 現在のシーンを取得
		Scene* currentScene = Manager::GetScene();
		// 現在のシーンの敵のオブジェクトを取得
		Enemy::HumanObject* enemy = currentScene->GetGameObject<Enemy::HumanObject>();

		m_PositionStart = enemy->GetPosition();
	}
}

CircleSkillShoot::~CircleSkillShoot()
{
}

void CircleSkillShoot::Load()
{
	ModelRenderer::Preload("asset\\model\\bullet.obj");
}

void CircleSkillShoot::Unload()
{
}

void CircleSkillShoot::Init()
{
	AddComponent<Shader>()->Load("shader\\PlayerShootBulletVS.cso", "shader\\EnemyShootBulletVS.cso", "shader\\vertexLightingPS.cso");
	AddComponent<ModelRenderer>()->Load("asset\\model\\bullet.obj");

	AddComponent<Shadow>()->SetSize(0.5f);

	m_Position.y += 2.0f;

	//子オブジェクトに当たり判定を追加
	m_Child = std::make_unique<BoundingSphereObj>(1, m_Position);

	//時間関係初期化
	m_ShootTime = std::make_unique<Timer::ScheduledTask>(0.5f);
}
void CircleSkillShoot::Update()
{
	try
	{
		if (ownerChara == CHARACTER::ENEMY)
		{
			Shader* shader = GetComponent<Shader>();

			shader->SetPlayerShder(false);
		}

		if (ownerChara == CHARACTER::PLAYER)
		{
			// 現在のシーンを取得
			Scene* currentScene = Manager::GetScene();
			// 現在のシーンのプレイヤーのオブジェクトを取得
			Player::PlayerObject* player = currentScene->GetGameObject<Player::PlayerObject>();
			// 現在のシーンの敵のオブジェクトを取得
			Enemy::HumanObject* enemy = currentScene->GetGameObject<Enemy::HumanObject>();

			if (m_ShootTime->GetFlg())
			{
				m_ShootMoveFlg = true;
			}

			if (!m_ShootMoveFlg)
			{
				// プレイヤーの位置を取得
				Vector3 playerPosition = player->GetPosition();

				// 玉をプレイヤーの右側に配置するためのオフセットを設定
				float offsetX = 1.0f; // 仮のオフセット値

				// 玉の位置を設定
				m_Position = playerPosition + Vector3::Right * offsetX;
				m_Position.y += 2.0f;
			}
			else
			{
				if (!m_EnemyPositionSaved)
				{
					// 敵の位置を取得
					m_EnemyPosition = enemy->GetPosition();
					m_EnemyPositionSaved = true;
				}

				// プレイヤーの位置を取得
				Vector3 playerPosition = player->GetPosition();

				// 玉が敵の位置に向かう方向を計算
				Vector3 directionToEnemy = m_EnemyPosition - m_Position;
				directionToEnemy.Normalize();

				// プレイヤーと敵の間のベクトルを計算
				Vector3 directionToPlayer = m_EnemyPosition - playerPosition;
				directionToPlayer.Normalize();

				// プレイヤーと敵の間のベクトルをy軸回りに度数分回転させる
				Matrix rotation = Matrix::CreateRotationY(DirectX::XMConvertToRadians(Degree));
				Vector3 rotatedDirection = Vector3::Transform(directionToPlayer, rotation);

				// 玉を発射する方向を設定
				Vector3 shootDirection = rotatedDirection;

				// 玉を発射する方向に移動させる
				m_Velocity = shootDirection * speed;
				m_Position += m_Velocity;
			}
		}
		else
		{
			//敵の場合

			// 現在のシーンを取得
			Scene* currentScene = Manager::GetScene();
			// 現在のシーンのプレイヤーのオブジェクトを取得
			Player::PlayerObject* player = currentScene->GetGameObject<Player::PlayerObject>();
			// 現在のシーンの敵のオブジェクトを取得
			Enemy::HumanObject* enemy = currentScene->GetGameObject<Enemy::HumanObject>();

			if (m_ShootTime->GetFlg())
			{
				m_ShootMoveFlg = true;
			}

			if (!m_ShootMoveFlg)
			{
				// 敵の位置を取得
				Vector3 enemyPosition = enemy->GetPosition();

				// 玉をプレイヤーの右側に配置するためのオフセットを設定
				float offsetX = 1.0f; // 仮のオフセット値

				// 玉の位置を設定
				m_Position = enemyPosition + Vector3::Right * offsetX;
				m_Position.y += 2.0f;
			}
			else
			{
				if (!m_EnemyPositionSaved)
				{
					// 敵の位置を取得
					m_EnemyPosition = enemy->GetPosition();
					m_EnemyPositionSaved = true;
				}

				// 敵の位置を取得
				Vector3 enemyPosition = enemy->GetPosition();

				// 玉が敵の位置に向かう方向を計算
				Vector3 directionToEnemy = m_Position - m_EnemyPosition;
				directionToEnemy.Normalize();

				// プレイヤーと敵の間のベクトルを計算
				Vector3 directionToPlayer = player->GetPosition() - m_EnemyPosition;
				directionToPlayer.Normalize();

				// プレイヤーと敵の間のベクトルをy軸回りに度数分回転させる
				Matrix rotation = Matrix::CreateRotationY(DirectX::XMConvertToRadians(Degree));
				Vector3 rotatedDirection = Vector3::Transform(directionToPlayer, rotation);

				// 玉を発射する方向を設定
				Vector3 shootDirection = rotatedDirection;

				// 玉を発射する方向に移動させる
				m_Velocity = shootDirection * speed;
				m_Position += m_Velocity;
			}
		}

		// 球の位置が一定範囲を超えた場合は削除
		if ((m_Position - m_PositionStart).Length() > 300.0f)
		{
			SetDestroy();
		}

		// 当たり判定の位置を更新
		Vector3 newpos = m_Position;
		newpos.y -= 2.0f;
		m_Child->SetCenter(newpos);
	}
	catch (const std::exception& e)
	{
		// 例外をキャッチし、エラーメッセージを出力
		std::cerr << "CircleSkillShoot::Update: " << e.what() << std::endl;
	}
}

void CircleSkillShoot::SetBulletOwner(enum class CHARACTER chara)
{
	ownerChara = chara;

	if (ownerChara == CHARACTER::PLAYER)
	{
		// 現在のシーンを取得
		Scene* currentScene = Manager::GetScene();
		// 現在のシーンのプレイヤーのオブジェクトを取得
		Player::PlayerObject* player = currentScene->GetGameObject<Player::PlayerObject>();

		m_PositionStart = player->GetPosition();
	}
	else if (ownerChara == CHARACTER::ENEMY)
	{
		// 現在のシーンを取得
		Scene* currentScene = Manager::GetScene();
		// 現在のシーンの敵のオブジェクトを取得
		Enemy::HumanObject* enemy = currentScene->GetGameObject<Enemy::HumanObject>();

		m_PositionStart = enemy->GetPosition();
	}
}

void CircleSkillShoot::SetBulletDegree(float degree)
{
	Degree = degree;
}

CHARACTER CircleSkillShoot::GetBulletOwner()
{
	return ownerChara;
}