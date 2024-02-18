#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "field.h"
#include "Player.h"
#include "game.h"
#include "input.h"
#include "audio.h"
#include "Polygon2D.h"
#include "manager.h"
#include "transition.h"
#include "result.h"
#include "utility.h"
#include "sky.h"
#include "score.h"
#include "explosion.h"
#include "Enemy.h"
#include "Application.h"
#include "sprite.h"
#include "title.h"
#include "HumanEnemy.h"
#include "ImguiManager.h"
#include <memory>

using namespace DirectX::SimpleMath;
using namespace Player;
using namespace Timer;
using namespace Enemy;

const float READYWIDTH = 256 * 2;
const float READYHEIGHT = 144 * 2;

const float SENTER_WIDTH = SCREEN_WIDTH / 2;
const float SENTER_HEIGHT = SCREEN_HEIGHT / 2;

// �Q�[����������
void Game::Init()
{
	Input::DisableInput();

	AddGameObject<Camera>(0);
	AddGameObject<Sky>(1);
	AddGameObject<Field>(1);
	AddGameObject<PlayerObject>(1);
	AddGameObject<HumanObject>(1);

	Scene* nowscene = Manager::GetScene();
	PlayerObject* player = nowscene->GetGameObject<PlayerObject>();
	HumanObject* enemy = nowscene->GetGameObject<HumanObject>();

	scoreObj.push_back(AddGameObject<Score>(3));
	scoreObj.at(0)->Init(enemy->GetHp());
	scoreObj.at(0)->SetPlace(300, 80, 50, 50, 30);

	scoreObj.push_back(AddGameObject<Score>(3));
	scoreObj.at(1)->Init(player->GetHp());
	scoreObj.at(1)->SetPlace(1000, 600, 70, 70, 40);

	AddGameObject<ImguiManager>(1);

	Ready = AddGameObject<GameObject>(3);
	Go = AddGameObject<GameObject>(3);

	Ready->AddComponent<Sprite>()->Init(SENTER_WIDTH - (READYWIDTH / 2), SENTER_HEIGHT - (READYHEIGHT / 2)
		, READYWIDTH, READYHEIGHT, "asset\\texture\\Ready.png");

	// ��ʑJ�ڃI�u�W�F�N�g��o�^
	m_Transition = AddGameObject<Transition>(3);
	m_Transition->FadeIn();

	//3.0��Ƀ^�X�N�����s����ScheduledTask���쐬
	scheduledTask = std::make_shared<ScheduledTask>(3.0f);
	m_GameOverTask = std::make_shared<ScheduledTask>();

	ImguiManager* imguiManager = nowscene->GetGameObject<ImguiManager>();
	imguiManager->Init(Application::GetHwnd());
}

// �Q�[���I������
void Game::Uninit()
{
	Scene* nowscene = Manager::GetScene();
	ImguiManager* imguiManager = nowscene->GetGameObject<ImguiManager>();
	imguiManager->End();
	Input::EnableInput();
}

// �Q�[���X�V����
void Game::Update()
{
	//�J�n���}
	ReadyGo();

	if (imguiEndflg == false)
	{
		Imgui();
	}
}

void Game::Draw()
{
}

void Game::ReadyGo()
{
	Scene* nowScene = Manager::GetScene();
	//���݂̃V�[���̃v���C���[�̃I�u�W�F�N�g���擾
	PlayerObject* player = nowScene->GetGameObject<PlayerObject>();
	HumanObject* enemy = nowScene->GetGameObject<HumanObject>();

	if (!m_GoEnd)
	{
		if (!m_Go)
		{
			// Get���\�b�h���ĂԂ��ƂŎw�肵�����Ԃ��o�߂���܂őҋ@���A���̌��true���擾
			if (!StartSetTime)
			{
				scheduledTask->SetTimer(3.0f);
				StartSetTime = true;
			}
			else
			{
				bool result = scheduledTask->GetFlg();

				//GO��\��
				if (result)
				{
					//�v���C���[�ƓG���A�N�e�B�u��ԂɕύX
					player->SetIsActive(true);
					enemy->SetIsActive(true);

					Go->AddComponent<Sprite>()->Init(SENTER_WIDTH - (READYWIDTH / 2), SENTER_HEIGHT - (READYHEIGHT / 2)
						, READYWIDTH, READYHEIGHT, "asset\\texture\\Go.png");

					Ready->SetDestroy();
					Input::EnableInput();
					scheduledTask->SetTimer(1.0f);
					m_Go = true;
				}
			}
		}
		else
		{
			bool result = scheduledTask->GetFlg();

			if (result)
			{
				Go->SetDestroy();
				m_GoEnd = true;
			}
		}
	}

	if (player->GetHp() <= 0 || enemy->GetHp() <= 0)
	{
		if (flg)
		{
			m_GameOverTask->SetTimer(2);
			flg = false;
			if (player->GetHp() <= 0)
			{
				win = true;
			}
			else
			{
				win = false;
			}
		}

		if (m_GameOverTask->GetFlg())
		{
			endflg = true;
		}

		if (endflg)
		{
			if (m_Transition->GetState() == Transition::State::Stop) {
				m_Transition->FadeOut();
			}

			// ��ʑJ�ڂ��I�����Ă邩�H
			if (m_Transition->GetState() == Transition::State::Finish)
			{
				Manager::SetScene<Result>();
				imguiEndflg = true;
			}
		}
	}
}

void Game::Imgui()
{
	Scene* nowscene = Manager::GetScene();
	ImguiManager* imguiManager = nowscene->GetGameObject<ImguiManager>();

	if (m_Transition->GetState() == Transition::State::Stop) {
		if (imguiManager->GetMapBool("TitleFlg"))
		{
			m_Transition->FadeOut();
		}
	}

	if (m_Transition->GetState() == Transition::State::Stop) {
		if (imguiManager->GetMapBool("ResultFlg"))
		{
			m_Transition->FadeOut();
		}
	}
	// ��ʑJ�ڂ��I�����Ă邩�H
	if (m_Transition->GetState() == Transition::State::Finish)
	{
		if (imguiManager->GetMapBool("TitleFlg"))
		{
			Manager::SetScene<Title>();
		}
		else if (imguiManager->GetMapBool("ResultFlg"))
		{
			Manager::SetScene<Result>();
		}
	}
}