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

using namespace DirectX::SimpleMath;
using namespace Player;
using namespace Timer;
using namespace Enemy;

const float READYWIDTH = 256 * 2;
const float READYHEIGHT = 144 * 2;

const float SENTER_WIDTH = SCREEN_WIDTH / 2;
const float SENTER_HEIGHT = SCREEN_HEIGHT / 2;

// ゲーム初期処理
void Game::Init()
{
	Input::DisableInput();

	AddGameObject<Camera>(0);
	AddGameObject<Sky>(1);
	AddGameObject<Field>(1);
	AddGameObject<PlayerObject>(1);
	AddGameObject<HumanObject>(1);
	AddGameObject<Score>(3);

	Ready = AddGameObject<GameObject>(3);
	Go = AddGameObject<GameObject>(3);

	Ready->AddComponent<Sprite>()->Init(SENTER_WIDTH - (READYWIDTH / 2), SENTER_HEIGHT - (READYHEIGHT / 2)
		, READYWIDTH, READYHEIGHT, "asset\\texture\\Ready.png");

	// 画面遷移オブジェクトを登録
	m_Transition = AddGameObject<Transition>(3);
	m_Transition->FadeIn();

	imguiManager = std::make_shared<ImguiManager>();

	//3.0後にタスクを実行するScheduledTaskを作成
	scheduledTask = std::make_shared<ScheduledTask>(3.0f);

	imguiManager->Init(Application::GetHwnd());
}

// ゲーム終了処理
void Game::Uninit()
{
	Input::EnableInput();
}

// ゲーム更新処理
void Game::Update()
{
	//開始合図
	ReadyGo();

	//imguiの処理
	Imgui();
}

void Game::Draw()
{
	if (imguiUpdateFlg)
	{
		imguiManager->Draw();
	}
}

void Game::ReadyGo()
{
	if (!m_GoEnd)
	{
		if (!m_Go)
		{
			// Getメソッドを呼ぶことで指定した時間が経過するまで待機し、その後にtrueを取得
			if (!StartSetTime)
			{
				scheduledTask->SetTimer(3.0f);
				StartSetTime = true;
			}
			else
			{
				bool result = scheduledTask->GetFlg();

				//GOを表示
				if (result)
				{
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
}

void Game::Imgui()
{
	//imguiの処理
	imguiManager->Update();

	//updateが読み込んだかどうか
	if (!imguiUpdateFlg)
	{
		imguiUpdateFlg = true;
	}

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
	// 画面遷移が終了してるか？
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