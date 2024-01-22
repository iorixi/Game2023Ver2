#include "main.h"
#include "manager.h"

#include "input.h"

#include "title.h"

#include "game.h"

#include "shader.h"
#include "sprite.h"

#include "transition.h"
#include "camera.h"
#include "sky.h"
#include"field.h"
#include "player.h"
#include "HumanEnemy.h"
#include "SelectCursol.h"

using namespace Player;
using namespace Enemy;

const int NUMCOMND = 3;
const float TARGETPOSXINIT = 800;

void Title::Init()
{
	GameObject* titleLogo = AddGameObject<GameObject>(3);			// 3はレイヤ番号
	titleLogo->AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	titleLogo->AddComponent<Sprite>()->Init(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, "asset\\texture\\title.jpg");

	titleChara = AddGameObject<GameObject>(3);
	titleChara->AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	titleChara->AddComponent<Sprite>()->Init(TARGETPOSXINIT, 200.0f, 894 / 2, 1283 / 2, "asset\\texture\\youmu.png");

	GameObject* titleEnter = AddGameObject<GameObject>(3);			// 3はレイヤ番号
	titleEnter->AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	titleEnter->AddComponent<Sprite>()->Init(300.0f, 200.0f, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, "asset\\texture\\Enter.png");

	//selectObj = AddGameObject<GameObject>(2);
	//selectObj->AddComponent<SelectCursol>()->Init(NUMCOMND);
	//
	//for (int i = 0; i < NUMCOMND; i++)
	//{
	//	SelectComand.at(i).AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	//}
	//
	//SelectComand.at(0).AddComponent<Sprite>()->Init(100.0f, 100.0f, 100, 100, "asset\\texture\\title.jpg");
	//SelectComand.at(1).AddComponent<Sprite>()->Init(200.0f, 200.0f, 100, 100, "asset\\texture\\title.jpg");
	//SelectComand.at(2).AddComponent<Sprite>()->Init(300.0f, 300.0f, 100, 100, "asset\\texture\\title.jpg");

	 // 新しく追加した変数
	moveSpeed = 0.30f;
	targetPosX = 700.0f; // ここに目標のX座標を設定

	m_Transition = AddGameObject<Transition>(3);					// 3はレイヤ番号
	m_Transition->FadeIn();
}

void Title::Update()
{
	// 画面遷移が終了しているか確認
	if (m_Transition->GetState() == Transition::State::Stop) {
		// エンターキーが押されたらフェードアウト
		if (Input::GetKeyTrigger(VK_RETURN)) {
			m_Transition->FadeOut();
		}

		Sprite* sprite = titleChara->GetComponent<Sprite>();

		// 800から700の間は通常の速度で移動
		if (sprite->GetPosX() > 700 && sprite->GetPosX() <= 800) {
			sprite->SetPos(sprite->GetPosX() - moveSpeed, sprite->GetPosY());
		}
		else {
			// それ以外の範囲では速度が200倍になり、画面の左端に来たら右端に移動
			sprite->SetPos(sprite->GetPosX() - (moveSpeed * 200.0f), sprite->GetPosY());

			// 画面の左端に来た瞬間
			if (sprite->GetPosX() <= -100) {
				// 通常の速度に戻す
				moveSpeed = 0.30f;
				// 右端に移動
				sprite->SetPos(SCREEN_WIDTH, sprite->GetPosY());
			}
		}
	}
	// 画面遷移が終了してるか？
	if (m_Transition->GetState() == Transition::State::Finish)
	{
		Manager::SetScene<Game>();
	}
}