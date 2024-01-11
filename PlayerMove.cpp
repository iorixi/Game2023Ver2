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

	Matrix viewMatrix = cameraObj->GetViewMatrix();
	Vector3 cameraForward = Vector3(viewMatrix._13, 0.0f, viewMatrix._33);

	Vector3 playerPosition = player->GetPosition();
	Vector3 playerRotation = player->GetRotation();

	// �O���x�N�g�����擾
	Vector3 playerForward = cameraForward;

	// �����x
	float acceleration = 0.01f;

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

	if (distanceToEnemy > maxDistance)
	{
		// �v���C���[�ƓG�̊Ԃ̃x�N�g��
		Vector3 playerToEnemy = enemy->GetPosition() - playerPosition;
		playerToEnemy.y = 0.0f; // ���������̉e���𖳎�
		playerToEnemy.Normalize(); // �x�N�g���̒�����1�ɐ��K��

		// �v���C���[�̉������x�N�g���i�E�����j
		Vector3 upVector = Vector3::Up;

		// �ڐ��x�N�g���i�E�����j
		Vector3 tangent = upVector.Cross(playerToEnemy);
		tangent.Normalize();

		// �㉺�����̈ړ���
		Vector3 verticalVec = playerForward * m_AroundMoveSpeed / 5;

		// ���E�����̈ړ���
		Vector3 horizontalVec = tangent * m_AroundMoveSpeed;

		float direction = 1;

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
				playerPosition -= verticalVec;
			}
		}
	}
	else
	{
		// �����ꍇ�͐ڐ����g���ĉ��ړ�

		// �v���C���[�ƓG�̊Ԃ̃x�N�g��
		Vector3 playerToEnemy = enemy->GetPosition() - playerPosition;
		playerToEnemy.y = 0.0f; // ���������̉e���𖳎�
		playerToEnemy.Normalize(); // �x�N�g���̒�����1�ɐ��K��

		// �v���C���[�̉������x�N�g���i�E�����j
		Vector3 upVector = Vector3::Up;

		// �ڐ��x�N�g���i�E�����j
		Vector3 tangent = upVector.Cross(playerToEnemy);
		tangent.Normalize();

		// �㉺�����̈ړ���
		Vector3 verticalVec = playerForward * m_AroundMoveSpeed / 5;

		// ���E�����̈ړ���
		Vector3 horizontalVec = tangent * m_AroundMoveSpeed;

		float direction = 1;

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
				playerPosition -= verticalVec;
			}
		}
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