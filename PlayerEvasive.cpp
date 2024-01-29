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
	// ���݂̃V�[�����擾
	Scene* currentScene = Manager::GetScene();
	// ���݂̃V�[���̃v���C���[�̃I�u�W�F�N�g���擾
	PlayerObject* player = currentScene->GetGameObject<PlayerObject>();
	Vector3 playerVelocity = player->GetVelocity();

	// ���݂̃V�[���̓G�̃I�u�W�F�N�g���擾
	HumanObject* enemy = currentScene->GetGameObject<HumanObject>();

	playerPosition = player->GetPosition();

	// �O���x�N�g�����擾
	playerForward = cameraForward;

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
	horizontalVec = tangent * m_EvasiveMoveSpeed;

	direction = 1;

	Vector3 playerRotation = player->GetRotation();
	//playerRotation.z -= 0.01f;

	if (Input::GetKeyTrigger(VK_SHIFT))
	{
		// �O�i����ь��
		if (Input::GetKeyPress('W'))
		{
			playerPosition += playerForward * (m_EvasiveMoveSpeed + acceleration);
		}
		if (Input::GetKeyPress('S'))
		{
			playerPosition -= playerForward * (m_EvasiveMoveSpeed + acceleration);
		}

		// ���E�ړ�
		if (Input::GetKeyPress('A') || Input::GetKeyPress('D'))
		{
			// �㉺�����Ɖ��Ǝ�O�����̈ړ���
			Vector3 verticalVec = playerForward * m_EvasiveMoveSpeed / 5;

			if (Input::GetKeyPress('D'))
			{
				direction *= -1;
			}

			playerPosition -= horizontalVec * direction;

			// A �L�[�܂��� D �L�[���P�Ƃŉ�����Ă���ꍇ
			if (!Input::GetKeyPress('S') && !Input::GetKeyPress('W'))
			{
				// �㉺�����Ɖ��Ǝ�O�����̈ړ���
				Vector3 verticalVec = playerForward * m_EvasiveMoveSpeed / 5;
				playerPosition -= verticalVec;
			}
		}
	}

	// �v���C���[�̍��W���X�V
	player->SetPosition(playerPosition);
	player->SetRotation(playerRotation);
	player->SetVelocity(playerVelocity);
}