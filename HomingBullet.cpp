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
#include "CharaEnum.h"
#include <iostream>
#include "CalculateAngle.h"

using namespace DirectX::SimpleMath;
using namespace Player;
using namespace Enemy;
using namespace Timer;

HomingBullet::HomingBullet()
{
	ownerChara = CHARACTER::NONE;
}

HomingBullet::HomingBullet(enum class CHARACTER chara)
{
	ownerChara = chara;
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
	m_Child = std::make_unique<BoundingSphereObj>(1, m_Position);

	//時間関係初期化
	m_HomingPointUpdateTime = std::make_shared<ScheduledTask>(homingPointUpdateTime);
	m_HomingTimeDestroy = std::make_shared<ScheduledTask>();
}

void HomingBullet::Update()
{
	try
	{
		//オーナーが設定されているか
		if (ownerChara != CHARACTER::NONE)
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
			float angleChange = 0.0f;

			// 球の追尾が有効の場合のみ処理を行う
			if (isActive)
			{
				//一定時間たったら処理
				//追尾位置更新
				if (m_HomingPointUpdateTime->GetFlg())
				{
					//プレイヤーなら
					if (ownerChara == CHARACTER::PLAYER)
					{
						// 球の移動方向を更新
						// 敵に向かうベクトルを計算
						directionToEnemy = enemyPosition - m_Position;
						directionToEnemy.y += 1.0f;

						directionToEnemy.Normalize();
						// 前のフレームのベクトルとの角度を計算
						angleChange = CalculateAngle(oldDirection, directionToEnemy);
						//ベクトルを保存
						oldDirection = directionToEnemy;
					}
					//敵なら
					else if (ownerChara == CHARACTER::ENEMY)
					{
						// 球の移動方向を更新
						// プレイヤーに向かうベクトルを計算
						directionToPlayer = playerPosition - m_Position;
						directionToPlayer.y += 1.0f;
						directionToPlayer.Normalize();
						// 前のフレームのベクトルとの角度を計算
						angleChange = CalculateAngle(oldDirection, directionToPlayer);
						//ベクトルを保存
						oldDirection = directionToPlayer;
					}

					// ベクトルの角度が一定角度以上変わった場合は無効にする
					if (angleChange >= ToRadians(overAngle)) {
						isActive = false;
					}
				}

				//プレイヤーなら
				if (ownerChara == CHARACTER::PLAYER)
				{
					// 敵と球の距離を計算
					float distanceToEnemy = Vector3::Distance(enemyPosition, m_Position);

					// 敵との距離が一定以上の場合は追尾する
					if (distanceToEnemy >= closeDistance)
					{
						// 方向にスピードをかける
						m_Velocity = directionToEnemy * speed;

						// 球の位置を更新
						m_Position += m_Velocity;
					}
					else
					{
						// ホーミング弾が一定距離以下の敵に接近した場合、追尾を無効にして直進する
						isActive = false;
						Vector3 forward = m_Velocity;
						forward.Normalize();

						// ホーミング弾が直進する方向を設定する
						m_Velocity = forward * speed; // forwardはホーミング弾の前方向ベクトルを表すものとします

						// 球の位置を更新
						m_Position += m_Velocity;
					}
				}
				else if (ownerChara == CHARACTER::ENEMY)
				{
					// 敵と球の距離を計算
					float distanceToPlayer = Vector3::Distance(playerPosition, m_Position);

					// 敵との距離が一定以上の場合は追尾する
					if (distanceToPlayer >= closeDistance)
					{
						// 方向にスピードをかける
						m_Velocity = directionToPlayer * speed;

						// 球の位置を更新
						m_Position += m_Velocity;
					}
					else
					{
						// ホーミング弾が一定距離以下の敵に接近した場合、追尾を無効にして直進する
						isActive = false;
						Vector3 forward = m_Velocity;
						forward.Normalize();

						// ホーミング弾が直進する方向を設定する
						m_Velocity = forward * speed; // forwardはホーミング弾の前方向ベクトルを表すものとします

						// 球の位置を更新
						m_Position += m_Velocity;
					}
				}
			}
			else
			{
				//追尾無効
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

			//球の消滅時間をセット
			if (m_timerSet == false)
			{
				m_HomingTimeDestroy->SetTimer(m_timer);
				m_timerSet = true;
			}
			// 指定時間経過で球消滅
			if (m_HomingTimeDestroy->GetFlg())
			{
				SetDestroy();
			}

			// 当たり判定の位置を更新
			m_Child->SetCenter(m_Position);
		}
		else
		{
			throw std::exception("オーナーキャラクターが設定されていません。");
		}
	}
	catch (const std::exception& e)
	{
		// 例外をキャッチし、適切な処理を行います
		std::cerr << "HomingBullet::Update で例外が発生しました: " << e.what() << std::endl;
	}
}

void HomingBullet::SetBulletOwner(enum class CHARACTER chara)
{
	ownerChara = chara;
}

CHARACTER HomingBullet::GetBulletOwner()
{
	return ownerChara;
}