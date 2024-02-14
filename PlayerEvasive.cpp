#include "PlayerEvasive.h"
#include "input.h"
#include "player.h"
#include "scene.h"
#include "manager.h"
#include "camera.h"
#include "HumanEnemy.h"
#include "ScheduledTask.h"
#include "ActionModo.h"

using namespace DirectX::SimpleMath;

void Player::Evasive::Init()
{
	evasiveModo = EvasiveModo::NONE;
	// m_ScheduledTask�̏�����
	m_EvasiveTime = std::make_shared<Timer::ScheduledTask>();

	m_EvasiveMoveSpeed = 0.5f;
	//evasiveTime = 0.2f;
	//coolDown = 0.1f;
}

void Player::Evasive::Update()
{
	// ���݂̃V�[�����擾
	Scene* currentScene = Manager::GetScene();
	// ���݂̃V�[���̃v���C���[�̃I�u�W�F�N�g���擾
	Player::PlayerObject* player = currentScene->GetGameObject<Player::PlayerObject>();
	Vector3 playerVelocity = player->GetVelocity();

	// ���݂̃V�[���̓G�̃I�u�W�F�N�g���擾
	Enemy::HumanObject* enemy = currentScene->GetGameObject<Enemy::HumanObject>();

	playerPosition = player->GetPosition();

	// ���݂̃V�[���̃J�����I�u�W�F�N�g���擾
	Camera* cameraObj = currentScene->GetGameObject<Camera>();

	viewMatrix = cameraObj->GetViewMatrix();
	cameraForward = Vector3(viewMatrix._13, 0.0f, viewMatrix._33);
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

	switch (evasiveModo)
	{
	case EvasiveModo::EVASIVE:
		EvasiveMove();
		break;

	case EvasiveModo::COOLDOWN:
		//�ŏ��ɓ�������
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

	//Vector3 playerRotation = player->GetRotation();
	//playerRotation.z += 0.01f;

	// �v���C���[�̍��W���X�V
	player->SetPosition(playerPosition);
	player->SetRotation(playerRotation);
	player->SetVelocity(playerVelocity);
}

void Player::Evasive::EvasiveMove()
{
	//�ŏ��ɓ�������
	if (m_InitFlgEvasiveTime)
	{
		m_EvasiveTime->SetTimer(evasiveTime);
		m_InitFlgEvasiveTime = false;

		//�A�j���V�����t���O���I���ɂ���
		animationFlg = true;
	}

	// �ړ����x�������x�ɉ����Ē�������
	float speed = m_EvasiveMoveSpeed * acceleration;

	Vector3 verticalVec = playerForward * speed;

	switch (evasiveVectol)
	{
	case EvasiveVectol::RIGHT:

		// �E�̈ړ���
		direction *= -1;
		playerPosition -= horizontalVec * direction;

		break;

	case EvasiveVectol::LEFT:

		// ���̈ړ���
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
	// ���݂̃V�[�����擾
	Scene* currentScene = Manager::GetScene();
	// ���݂̃V�[���̃v���C���[�̃I�u�W�F�N�g���擾
	PlayerObject* player = currentScene->GetGameObject<PlayerObject>();
	player->SetActionModo(ActionModo::EVASIVE);

	//��莞�Ԍo�܂ŏ���
	if (m_EvasiveTime->GetFlg())
	{
		evasiveModo = EvasiveModo::COOLDOWN;
		m_InitFlgEvasiveTime = true;
	}
}

void Player::Evasive::EvasiveDirection()
{
	//SHIFT�������ꂽ��
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