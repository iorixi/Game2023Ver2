#include "PlayerFloating.h"
#include "input.h"
#include "player.h"
#include "scene.h"
#include "manager.h"
#include "camera.h"
#include "ActionModo.h"

using namespace DirectX::SimpleMath;
using namespace Player;

void Floating::Init()
{
}

void Floating::Update()
{
	//���݂̃V�[�����擾
	Scene* scene = Manager::GetScene();
	//���݂̃V�[���̃v���C���[�̃I�u�W�F�N�g���擾
	PlayerObject* player = scene->GetGameObject<PlayerObject>();

	Vector3 playerVelocity = player->GetVelocity();

	//�㏸or���~�L�[�������ꂽ��
	if (Input::GetKeyPress(VK_SPACE) || Input::GetKeyPress(VK_CONTROL))
	{
		//�㏸
		if (Input::GetKeyPress(VK_SPACE))
		{
			playerVelocity.y = 0.16f;

			player->SetActionModo(ActionModo::MOVE);
		}

		//���~
		if (Input::GetKeyPress(VK_CONTROL))
		{
			playerVelocity.y = -0.16f;

			player->SetActionModo(ActionModo::MOVE);
		}
	}
	else if (m_StandRise)
	{
		playerVelocity.y = 0.0f;

		m_StandFloatingDistance += m_StandSpeed;

		if (m_StandFloatingDistance > m_StandFloatingDistanceMax)
		{
			m_StandRise = false;
		}
	}
	else if (!m_StandRise)
	{
		playerVelocity.y = 0.0f;

		m_StandFloatingDistance -= m_StandSpeed;

		if (m_StandFloatingDistance < (m_StandFloatingDistanceMax * -1))
		{
			m_StandRise = true;
		}
	}

	playerVelocity.y += m_StandFloatingDistance;

	player->SetVelocity(playerVelocity);
}