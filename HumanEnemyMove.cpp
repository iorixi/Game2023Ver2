#include "HumanEnemyMove.h"
#include "Input.h"
#include "Player.h"
#include "Scene.h"
#include "Manager.h"
#include "Camera.h"
#include "HumanEnemy.h"
#include "ImguiManager.h"
#include "ActionModo.h"

using namespace DirectX::SimpleMath;

void Enemy::Move::Init()
{
	moveModo = MoveModo::NONE;
}

void Enemy::Move::Update()
{
	// 現在のシーンを取得
	Scene* currentScene = Manager::GetScene();
	// 現在のシーンのプレイヤーのオブジェクトを取得
	Player::PlayerObject* player = currentScene->GetGameObject<Player::PlayerObject>();

	// 現在のシーンの敵のオブジェクトを取得
	HumanObject* enemy = currentScene->GetGameObject<HumanObject>();

	// 現在のシーンのカメラオブジェクトを取得
	Camera* cameraObj = currentScene->GetGameObject<Camera>();

	viewMatrix = cameraObj->GetViewMatrix();
	cameraForward = Vector3(viewMatrix._13, 0.0f, viewMatrix._33);

	enemyPosition = enemy->GetPosition();
	enemyRotation = enemy->GetRotation();

	// 前方ベクトルを取得
	enemyForward = cameraForward;

	// 前進および後退
	if (Input::GetKeyPress('W'))
	{
		enemyPosition += enemyForward * (m_AroundMoveSpeed + acceleration);
	}
	if (Input::GetKeyPress('S'))
	{
		enemyPosition -= enemyForward * (m_AroundMoveSpeed + acceleration);
	}

	// 移動速度に補正がかかる最大の距離
	float maxDistance = 10.0f;

	// 左右移動を円状にするための角度の変化量
	float angleDelta = 0.02f;

	// プレイヤーと敵の距離を計算
	float distanceToEnemy = Vector3::Distance(enemyPosition, player->GetPosition());

	// プレイヤーの現在の角度（ラジアン単位）を計算
	float enemyAngle = atan2(enemyPosition.z - enemy->GetPosition().z, enemyPosition.x - player->GetPosition().x);

	// 接触する距離
	float collisionDistance = 20.0f;

	// プレイヤーと敵の間のベクトル
	enemyToPlayer = player->GetPosition() - enemyPosition;
	enemyToPlayer.y = 0.0f; // 高さ方向の影響を無視
	enemyToPlayer.Normalize(); // ベクトルの長さを1に正規化

	// プレイヤーの横向きベクトル（右向き）
	upVector = Vector3::Up;

	// 接線ベクトル（右向き）
	tangent = upVector.Cross(enemyToPlayer);
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

	// プレイヤーの座標を更新
	enemy->SetPosition(enemyPosition);
	enemy->SetRotation(enemyRotation);
}

void Enemy::Move::FarDistance()
{
	// 左右移動
	if (Input::GetKeyPress('A') || Input::GetKeyPress('D'))
	{
		// 上下方向と奥と手前方向の移動量
		Vector3 verticalVec = enemyForward * m_AroundMoveSpeed / 5;

		if (Input::GetKeyPress('D'))
		{
			direction *= -1;
		}

		enemyPosition += horizontalVec * direction;

		// A キーまたは D キーが単独で押されている場合
		if (!Input::GetKeyPress('S') && !Input::GetKeyPress('W'))
		{
			// 上下方向と奥と手前方向の移動量
			Vector3 verticalVec = enemyForward * m_AroundMoveSpeed / 5;
			enemyPosition += verticalVec;
		}

		// 現在のシーンを取得
		Scene* currentScene = Manager::GetScene();
		// 現在のシーンのプレイヤーのオブジェクトを取得
		HumanObject* enemy = currentScene->GetGameObject<HumanObject>();
		enemy->SetActionModo(ActionModo::MOVE);
	}
}

void Enemy::Move::CloseDistance()
{
	// 左右移動
	if (Input::GetKeyPress('A') || Input::GetKeyPress('D'))
	{
		if (Input::GetKeyPress('D'))
		{
			direction *= -1;
		}
		enemyPosition -= horizontalVec * direction;
		moveVec = horizontalVec * direction;

		// A キーまたは D キーが単独で押されている場合
		if (!Input::GetKeyPress('S') && !Input::GetKeyPress('W'))
		{
			// 上下方向と奥と手前方向の移動量
			Vector3 verticalVec = enemyForward * m_AroundMoveSpeed / 5;
			enemyPosition += verticalVec;
			// 垂直方向の移動量を moveVec に加算
			moveVec += verticalVec;
		}
		// 現在のシーンを取得
		Scene* currentScene = Manager::GetScene();
		// 現在のシーンのプレイヤーのオブジェクトを取得
		HumanObject* enemy = currentScene->GetGameObject<HumanObject>();
		enemy->SetActionModo(ActionModo::MOVE);
	}
}

MoveModo Enemy::Move::GetEnemyMoveModo()
{
	return moveModo;
}