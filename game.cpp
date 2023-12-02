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

using namespace DirectX::SimpleMath;
using namespace Player;

// ゲーム初期処理
void Game::Init()
{
	AddGameObject<Camera>(0);
	AddGameObject<Sky>(1);
	AddGameObject<Field>(1);
	AddGameObject<PlayerObject>(1);
	AddGameObject<Enemy>(1);
	AddGameObject<Score>(3);

	// 画面遷移オブジェクトを登録
	m_Transition = AddGameObject<Transition>(3);
	m_Transition->FadeIn();

	imguiManager = std::make_unique<ImguiManager>();

	imguiManager->Init(Application::GetHwnd());
}

// ゲーム終了処理
void Game::Uninit()
{
}

// ゲーム更新処理
void Game::Update()
{
	// フェードアウトが終了しているか？
	if (m_Transition->GetState() == Transition::State::Finish)
	{
		Manager::SetScene<Result>();
	}

	imguiManager->Update();
	//updateが読み込んだかどうか
	if (!imguiUpdateFlg)
	{
		imguiUpdateFlg = true;
	}
}

void Game::Draw()
{
	if (imguiUpdateFlg)
	{
		imguiManager->Draw();
	}
}