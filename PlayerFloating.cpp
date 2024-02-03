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
	//現在のシーンを取得
	Scene* scene = Manager::GetScene();
	//現在のシーンのプレイヤーのオブジェクトを取得
	PlayerObject* player = scene->GetGameObject<PlayerObject>();

	Vector3 playerVelocity = player->GetVelocity();

	//上昇or下降キーが押されたか
	if (Input::GetKeyPress(VK_SPACE) || Input::GetKeyPress(VK_CONTROL))
	{
		//上昇
		if (Input::GetKeyPress(VK_SPACE))
		{
			playerVelocity.y = 0.16f;

			player->SetActionModo(ActionModo::MOVE);
		}

		//下降
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