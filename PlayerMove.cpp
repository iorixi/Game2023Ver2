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
	// ���݂̃V�[�����擾
	Scene* currentScene = Manager::GetScene();
	// ���݂̃V�[���̃v���C���[�̃I�u�W�F�N�g���擾
	PlayerObject* player = currentScene->GetGameObject<PlayerObject>();

	// ���݂̃V�[���̓G�̃I�u�W�F�N�g���擾
	HumanObject* enemy = currentScene->GetGameObject<HumanObject>();

	// ���݂̃V�[���̃J�����I�u�W�F�N�g���擾
	Camera* cameraObj = currentScene->GetGameObject<Camera>();

	viewMatrix = cameraObj->GetViewMatrix();
	cameraForward = Vector3(viewMatrix._13, 0.0f, viewMatrix._33);

	playerPosition = player->GetPosition();
	playerRotation = player->GetRotation();

	// �O���x�N�g�����擾
	playerForward = cameraForward;

	// �O�i����ь��
	if (Input::GetKeyPress('W'))
	{
		playerPosition += playerForward * (m_AroundMoveSpeed + acceleration);
	}
	if (Input::GetKeyPress('S'))
	{
		playerPosition -= playerForward * (m_AroundMoveSpeed + acceleration);
	}

	// �ړ����x�ɕ␳��������ő�̋���
	float maxDistance = 10.0f;

	// ���E�ړ����~��ɂ��邽�߂̊p�x�̕ω���
	float angleDelta = 0.02f;

	// �v���C���[�ƓG�̋������v�Z
	float distanceToEnemy = Vector3::Distance(playerPosition, enemy->GetPosition());

	// �v���C���[�̌��݂̊p�x�i���W�A���P�ʁj���v�Z
	float playerAngle = atan2(playerPosition.z - enemy->GetPosition().z, playerPosition.x - enemy->GetPosition().x);

	// �ڐG���鋗��
	float collisionDistance = 20.0f;

	// �v���C���[�ƓG�̊Ԃ̃x�N�g��
	playerToEnemy = enemy->GetPosition() - playerPosition;
	playerToEnemy.y = 0.0f; // ���������̉e���𖳎�
	playerToEnemy.Normalize(); // �x�N�g���̒�����1�ɐ��K��

	// �v���C���[�̉������x�N�g���i�E�����j
	upVector = Vector3::Up;

	// �ڐ��x�N�g���i�E�����j
	tangent = upVector.Cross(playerToEnemy);
	tangent.Normalize();

	// ���E�����̈ړ���
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

	// �����L�[
	if (Input::GetKeyPress(VK_LEFT))
	{
		playerRotation.y -= 0.01f;
	}

	// �E���L�[
	if (Input::GetKeyPress(VK_RIGHT))
	{
		playerRotation.y += 0.01f;
	}

	// ���݂̃V�[���̃v���C���[�̃I�u�W�F�N�g���擾
	ImguiManager* imguiManager = currentScene->GetGameObject<ImguiManager>();

	// Calculate and set the distance between player and enemy
	imguiManager->SetDistance(distanceToEnemy);

	// �v���C���[�̍��W���X�V
	player->SetPosition(playerPosition);
	player->SetRotation(playerRotation);
}

void Player::Move::FarDistance()
{
	// ���E�ړ�
	if (Input::GetKeyPress('A') || Input::GetKeyPress('D'))
	{
		// �㉺�����Ɖ��Ǝ�O�����̈ړ���
		Vector3 verticalVec = playerForward * m_AroundMoveSpeed / 5;

		if (Input::GetKeyPress('D'))
		{
			direction *= -1;
		}

		playerPosition -= horizontalVec * direction;

		// A �L�[�܂��� D �L�[���P�Ƃŉ�����Ă���ꍇ
		if (!Input::GetKeyPress('S') && !Input::GetKeyPress('W'))
		{
			// �㉺�����Ɖ��Ǝ�O�����̈ړ���
			Vector3 verticalVec = playerForward * m_AroundMoveSpeed / 5;
			playerPosition -= verticalVec;
		}
	}
}

void Player::Move::CloseDistance()
{
	// ���E�ړ�
	if (Input::GetKeyPress('A') || Input::GetKeyPress('D'))
	{
		if (Input::GetKeyPress('D'))
		{
			direction *= -1;
		}
		playerPosition -= horizontalVec * direction;

		// A �L�[�܂��� D �L�[���P�Ƃŉ�����Ă���ꍇ
		if (!Input::GetKeyPress('S') && !Input::GetKeyPress('W'))
		{
			// �㉺�����Ɖ��Ǝ�O�����̈ړ���
			Vector3 verticalVec = playerForward * m_AroundMoveSpeed / 5;
			playerPosition -= verticalVec;
		}
	}
}

MoveModo Player::Move::GetPlayerMoveModo()
{
	return moveModo;
}