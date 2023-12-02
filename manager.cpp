#include	"main.h"
#include	"renderer.h"
#include	"manager.h"
#include	"audio.h"
#include	"input.h"
#include	"scene.h"
#include	"game.h"
#include	"title.h"

// シーンマネージャ内メンバ変数の実体
Scene* Manager::m_Scene{};					// 現在シーン

void Manager::Init(Application* ap)
{
	// タイマ解像度を１ミリ秒にする
	timeBeginPeriod(1);

	// レンダラーを初期化
	Renderer::Init(ap);

	// 音初期化
	Sound::Audio::InitMaster();

	// 入力を初期化
	Input::Init();

	// Titleを初期シーンに登録
	SetScene<Title>();
}

void Manager::Uninit()
{
	m_Scene->UninitBase();
	delete m_Scene;

	// ３Ｄモデルを全て解放
	ModelRenderer::UnloadAll();

	// インプット終了
	Input::Uninit();

	// オーディオ終了
	Sound::Audio::UninitMaster();

	// タイマ解像度を元に戻す
	timeEndPeriod(1);
}

void Manager::Update(uint64_t d)
{
	Input::Update();

	m_Scene->UpdateBase();
}

void Manager::Draw(uint64_t d)
{
	Renderer::Begin();
	m_Scene->DrawBase();

	Renderer::End();
}