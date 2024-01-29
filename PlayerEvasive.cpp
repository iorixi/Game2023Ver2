#include "PlayerEvasive.h"
#include "input.h"
#include "player.h"
#include "scene.h"
#include "manager.h"
#include "camera.h"
#include "HumanEnemy.h"

using namespace DirectX::SimpleMath;
using namespace Player;
using namespace Enemy;

void Evasive::Init()
{
}

void Evasive::Update()
{
	// 現在のシーンを取得
	Scene* currentScene = Manager::GetScene();
	// 現在のシーンのプレイヤーのオブジェクトを取得
	PlayerObject* player = currentScene->GetGameObject<PlayerObject>();
	Vector3 playerVelocity = player->GetVelocity();

	// 現在のシーンの敵のオブジェクトを取得
	HumanObject* enemy = currentScene->GetGameObject<HumanObject>();

	playerPosition = player->GetPosition();

	// 前方ベクトルを取得
	playerForward = cameraForward;

	// プレイヤーと敵の間のベクトル
	playerToEnemy = enemy->GetPosition() - playerPosition;
	playerToEnemy.y = 0.0f; // 高さ方向の影響を無視
	playerToEnemy.Normalize(); // ベクトルの長さを1に正規化

	// プレイヤーの横向きベクトル（右向き）
	upVector = Vector3::Up;

	// 接線ベクトル（右向き）
	tangent = upVector.Cross(playerToEnemy);
	tangent.Normalize();

	// 左右方向の移動量
	horizontalVec = tangent * m_EvasiveMoveSpeed;

	direction = 1;

	Vector3 playerRotation = player->GetRotation();
	//playerRotation.z -= 0.01f;

	if (Input::GetKeyTrigger(VK_SHIFT))
	{
		// 前進および後退
		if (Input::GetKeyPress('W'))
		{
			playerPosition += playerForward * (m_EvasiveMoveSpeed + acceleration);
		}
		if (Input::GetKeyPress('S'))
		{
			playerPosition -= playerForward * (m_EvasiveMoveSpeed + acceleration);
		}

		// 左右移動
		if (Input::GetKeyPress('A') || Input::GetKeyPress('D'))
		{
			// 上下方向と奥と手前方向の移動量
			Vector3 verticalVec = playerForward * m_EvasiveMoveSpeed / 5;

			if (Input::GetKeyPress('D'))
			{
				direction *= -1;
			}

			playerPosition -= horizontalVec * direction;

			// A キーまたは D キーが単独で押されている場合
			if (!Input::GetKeyPress('S') && !Input::GetKeyPress('W'))
			{
				// 上下方向と奥と手前方向の移動量
				Vector3 verticalVec = playerForward * m_EvasiveMoveSpeed / 5;
				playerPosition -= verticalVec;
			}
		}
	}

	// プレイヤーの座標を更新
	player->SetPosition(playerPosition);
	player->SetRotation(playerRotation);
	player->SetVelocity(playerVelocity);
}