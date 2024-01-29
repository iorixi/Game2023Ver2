#include "PlayerEvasive.h"
#include "input.h"
#include "player.h"
#include "scene.h"
#include "manager.h"
#include "camera.h"
#include "HumanEnemy.h"
#include "ScheduledTask.h"

using namespace DirectX::SimpleMath;
using namespace Player;
using namespace Enemy;
using namespace Timer;

void Evasive::Init()
{
	evasiveModo = EvasiveModo::NONE;
	// m_ScheduledTaskの初期化
	m_EvasiveTime = std::make_shared<ScheduledTask>();

	m_EvasiveMoveSpeed = 0.5f;
	//evasiveTime = 0.2f;
	//coolDown = 0.1f;
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

	// 現在のシーンのカメラオブジェクトを取得
	Camera* cameraObj = currentScene->GetGameObject<Camera>();

	viewMatrix = cameraObj->GetViewMatrix();
	cameraForward = Vector3(viewMatrix._13, 0.0f, viewMatrix._33);
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

	switch (evasiveModo)
	{
	case EvasiveModo::EVASIVE:
		EvasiveMove();
		break;

	case EvasiveModo::COOLDOWN:
		//最初に入った時
		if (m_InitFlgEvasiveCoolTime)
		{
			m_EvasiveTime->SetTimer(coolDown);
			m_InitFlgEvasiveCoolTime = false;
		}

		if (m_EvasiveTime->GetFlg())
		{
			evasiveModo = EvasiveModo::NONE;

			m_InitFlgEvasiveCoolTime = true;
		}
		break;

	case EvasiveModo::NONE:

		EvasiveDirection();
		break;
	default:
		break;
	}//switch

	// プレイヤーの座標を更新
	player->SetPosition(playerPosition);
	player->SetRotation(playerRotation);
	player->SetVelocity(playerVelocity);
}

void Player::Evasive::EvasiveMove()
{
	//最初に入った時
	if (m_InitFlgEvasiveTime)
	{
		m_EvasiveTime->SetTimer(evasiveTime);
		m_InitFlgEvasiveTime = false;
	}

	// 移動速度を加速度に応じて調整する
	float speed = m_EvasiveMoveSpeed * acceleration;

	Vector3 verticalVec = playerForward * speed;

	switch (evasiveVectol)
	{
	case EvasiveVectol::RIGHT:

		// 右の移動量
		direction *= -1;
		playerPosition -= horizontalVec * direction;

		break;

	case EvasiveVectol::LEFT:

		// 左の移動量
		playerPosition -= horizontalVec * direction;

		break;

	case EvasiveVectol::FORWARD:

		playerPosition += playerForward * (m_EvasiveMoveSpeed * acceleration);

		break;

	case EvasiveVectol::BACKWADO:

		playerPosition -= playerForward * (m_EvasiveMoveSpeed * acceleration);

		break;
	default:
		break;
	}

	//一定時間経つまで処理
	if (m_EvasiveTime->GetFlg())
	{
		evasiveModo = EvasiveModo::COOLDOWN;
		m_InitFlgEvasiveTime = true;
	}
}

void Player::Evasive::EvasiveDirection()
{
	//SHIFTが押されたら
	if (Input::GetKeyTrigger(VK_SHIFT))
	{
		if (Input::GetKeyPress('A') && Input::GetKeyPress('D'))
		{
		}
		else if (Input::GetKeyPress('A'))
		{
			evasiveModo = EvasiveModo::EVASIVE;
			evasiveVectol = EvasiveVectol::LEFT;
		}
		else if (Input::GetKeyPress('D'))
		{
			evasiveModo = EvasiveModo::EVASIVE;
			evasiveVectol = EvasiveVectol::RIGHT;
		}
		else if (Input::GetKeyPress('W'))
		{
			evasiveModo = EvasiveModo::EVASIVE;
			evasiveVectol = EvasiveVectol::FORWARD;
		}
		else if (Input::GetKeyPress('S'))
		{
			evasiveModo = EvasiveModo::EVASIVE;
			evasiveVectol = EvasiveVectol::BACKWADO;
		}
		else
		{
		}
	}
}