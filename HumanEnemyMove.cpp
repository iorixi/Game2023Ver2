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
	// ���݂̃V�[�����擾
	Scene* currentScene = Manager::GetScene();
	// ���݂̃V�[���̃v���C���[�̃I�u�W�F�N�g���擾
	Player::PlayerObject* player = currentScene->GetGameObject<Player::PlayerObject>();

	// ���݂̃V�[���̓G�̃I�u�W�F�N�g���擾
	HumanObject* enemy = currentScene->GetGameObject<HumanObject>();

	// ���݂̃V�[���̃J�����I�u�W�F�N�g���擾
	Camera* cameraObj = currentScene->GetGameObject<Camera>();

	viewMatrix = cameraObj->GetViewMatrix();
	cameraForward = Vector3(viewMatrix._13, 0.0f, viewMatrix._33);

	enemyPosition = enemy->GetPosition();
	enemyRotation = enemy->GetRotation();

	// �O���x�N�g�����擾
	enemyForward = cameraForward;

	// �O�i����ь��
	if (Input::GetKeyPress('W'))
	{
		enemyPosition += enemyForward * (m_AroundMoveSpeed + acceleration);
	}
	if (Input::GetKeyPress('S'))
	{
		enemyPosition -= enemyForward * (m_AroundMoveSpeed + acceleration);
	}

	// �ړ����x�ɕ␳��������ő�̋���
	float maxDistance = 10.0f;

	// ���E�ړ����~��ɂ��邽�߂̊p�x�̕ω���
	float angleDelta = 0.02f;

	// �v���C���[�ƓG�̋������v�Z
	float distanceToEnemy = Vector3::Distance(enemyPosition, player->GetPosition());

	// �v���C���[�̌��݂̊p�x�i���W�A���P�ʁj���v�Z
	float enemyAngle = atan2(enemyPosition.z - enemy->GetPosition().z, enemyPosition.x - player->GetPosition().x);

	// �ڐG���鋗��
	float collisionDistance = 20.0f;

	// �v���C���[�ƓG�̊Ԃ̃x�N�g��
	enemyToPlayer = player->GetPosition() - enemyPosition;
	enemyToPlayer.y = 0.0f; // ���������̉e���𖳎�
	enemyToPlayer.Normalize(); // �x�N�g���̒�����1�ɐ��K��

	// �v���C���[�̉������x�N�g���i�E�����j
	upVector = Vector3::Up;

	// �ڐ��x�N�g���i�E�����j
	tangent = upVector.Cross(enemyToPlayer);
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

	// �v���C���[�̍��W���X�V
	enemy->SetPosition(enemyPosition);
	enemy->SetRotation(enemyRotation);
}

void Enemy::Move::FarDistance()
{
	// ���E�ړ�
	if (Input::GetKeyPress('A') || Input::GetKeyPress('D'))
	{
		// �㉺�����Ɖ��Ǝ�O�����̈ړ���
		Vector3 verticalVec = enemyForward * m_AroundMoveSpeed / 5;

		if (Input::GetKeyPress('D'))
		{
			direction *= -1;
		}

		enemyPosition += horizontalVec * direction;

		// A �L�[�܂��� D �L�[���P�Ƃŉ�����Ă���ꍇ
		if (!Input::GetKeyPress('S') && !Input::GetKeyPress('W'))
		{
			// �㉺�����Ɖ��Ǝ�O�����̈ړ���
			Vector3 verticalVec = enemyForward * m_AroundMoveSpeed / 5;
			enemyPosition += verticalVec;
		}

		// ���݂̃V�[�����擾
		Scene* currentScene = Manager::GetScene();
		// ���݂̃V�[���̃v���C���[�̃I�u�W�F�N�g���擾
		HumanObject* enemy = currentScene->GetGameObject<HumanObject>();
		enemy->SetActionModo(ActionModo::MOVE);
	}
}

void Enemy::Move::CloseDistance()
{
	// ���E�ړ�
	if (Input::GetKeyPress('A') || Input::GetKeyPress('D'))
	{
		if (Input::GetKeyPress('D'))
		{
			direction *= -1;
		}
		enemyPosition -= horizontalVec * direction;
		moveVec = horizontalVec * direction;

		// A �L�[�܂��� D �L�[���P�Ƃŉ�����Ă���ꍇ
		if (!Input::GetKeyPress('S') && !Input::GetKeyPress('W'))
		{
			// �㉺�����Ɖ��Ǝ�O�����̈ړ���
			Vector3 verticalVec = enemyForward * m_AroundMoveSpeed / 5;
			enemyPosition += verticalVec;
			// ���������̈ړ��ʂ� moveVec �ɉ��Z
			moveVec += verticalVec;
		}
		// ���݂̃V�[�����擾
		Scene* currentScene = Manager::GetScene();
		// ���݂̃V�[���̃v���C���[�̃I�u�W�F�N�g���擾
		HumanObject* enemy = currentScene->GetGameObject<HumanObject>();
		enemy->SetActionModo(ActionModo::MOVE);
	}
}

MoveModo Enemy::Move::GetEnemyMoveModo()
{
	return moveModo;
}