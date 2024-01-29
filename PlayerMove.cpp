#include "PlayerMove.h"
#include "Input.h"
#include "Player.h"
#include "Scene.h"
#include "Manager.h"
#include "Camera.h"
#include "HumanEnemy.h"
#include "ImguiManager.h"

using namespace DirectX::SimpleMath;
using namespace Player;
using namespace Enemy;

void Move::Init()
{
	moveModo = MoveModo::NONE;
}

void Move::Update()
{
	// 現在のシーンを取得
	Scene* currentScene = Manager::GetScene();
	// 現在のシーンのプレイヤーのオブジェクトを取得
	PlayerObject* player = currentScene->GetGameObject<PlayerObject>();

	// 現在のシーンの敵のオブジェクトを取得
	HumanObject* enemy = currentScene->GetGameObject<HumanObject>();

	// 現在のシーンのカメラオブジェクトを取得
	Camera* cameraObj = currentScene->GetGameObject<Camera>();

	viewMatrix = cameraObj->GetViewMatrix();
	cameraForward = Vector3(viewMatrix._13, 0.0f, viewMatrix._33);

	playerPosition = player->GetPosition();
	playerRotation = player->GetRotation();

	// 前方ベクトルを取得
	playerForward = cameraForward;

	// 前進および後退
	if (Input::GetKeyPress('W'))
	{
		playerPosition += playerForward * (m_AroundMoveSpeed + acceleration);
	}
	if (Input::GetKeyPress('S'))
	{
		playerPosition -= playerForward * (m_AroundMoveSpeed + acceleration);
	}

	// 移動速度に補正がかかる最大の距離
	float maxDistance = 10.0f;

	// 左右移動を円状にするための角度の変化量
	float angleDelta = 0.02f;

	// プレイヤーと敵の距離を計算
	float distanceToEnemy = Vector3::Distance(playerPosition, enemy->GetPosition());

	// プレイヤーの現在の角度（ラジアン単位）を計算
	float playerAngle = atan2(playerPosition.z - enemy->GetPosition().z, playerPosition.x - enemy->GetPosition().x);

	// 接触する距離
	float collisionDistance = 20.0f;

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
	horizontalVec = tangent * m_AroundMoveSpeed;

	direction = 1;

	if (distanceToEnemy > maxDistance)
	{
		moveModo = MoveModo::FARDISTANCE;
	}
	else
	{
		moveModo = MoveModo::CLOSEDISTANCE;
	}

	switch (moveModo)
	{
	case MoveModo::FARDISTANCE:
		FarDistance();
		break;
	case MoveModo::CLOSEDISTANCE:
		CloseDistance();
		break;
	case MoveModo::SPECIAL:
		break;
	case MoveModo::NONE:
		break;
	default:
		break;
	}

	// 左矢印キー
	if (Input::GetKeyPress(VK_LEFT))
	{
		playerRotation.y -= 0.01f;
	}

	// 右矢印キー
	if (Input::GetKeyPress(VK_RIGHT))
	{
		playerRotation.y += 0.01f;
	}

	// 現在のシーンのプレイヤーのオブジェクトを取得
	ImguiManager* imguiManager = currentScene->GetGameObject<ImguiManager>();

	// Calculate and set the distance between player and enemy
	imguiManager->SetDistance(distanceToEnemy);

	// プレイヤーの座標を更新
	player->SetPosition(playerPosition);
	player->SetRotation(playerRotation);
}

void Player::Move::FarDistance()
{
	// 左右移動
	if (Input::GetKeyPress('A') || Input::GetKeyPress('D'))
	{
		// 上下方向と奥と手前方向の移動量
		Vector3 verticalVec = playerForward * m_AroundMoveSpeed / 5;

		if (Input::GetKeyPress('D'))
		{
			direction *= -1;
		}

		playerPosition -= horizontalVec * direction;

		// A キーまたは D キーが単独で押されている場合
		if (!Input::GetKeyPress('S') && !Input::GetKeyPress('W'))
		{
			// 上下方向と奥と手前方向の移動量
			Vector3 verticalVec = playerForward * m_AroundMoveSpeed / 5;
			playerPosition -= verticalVec;
		}
	}
}

void Player::Move::CloseDistance()
{
	// 左右移動
	if (Input::GetKeyPress('A') || Input::GetKeyPress('D'))
	{
		if (Input::GetKeyPress('D'))
		{
			direction *= -1;
		}
		playerPosition -= horizontalVec * direction;

		// A キーまたは D キーが単独で押されている場合
		if (!Input::GetKeyPress('S') && !Input::GetKeyPress('W'))
		{
			// 上下方向と奥と手前方向の移動量
			Vector3 verticalVec = playerForward * m_AroundMoveSpeed / 5;
			playerPosition -= verticalVec;
		}
	}
}

MoveModo Player::Move::GetPlayerMoveModo()
{
	return moveModo;
}