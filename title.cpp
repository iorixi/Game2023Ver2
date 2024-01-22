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
	GameObject* titleLogo = AddGameObject<GameObject>(3);			// 3�̓��C���ԍ�
	titleLogo->AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	titleLogo->AddComponent<Sprite>()->Init(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, "asset\\texture\\title.jpg");

	titleChara = AddGameObject<GameObject>(3);
	titleChara->AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	titleChara->AddComponent<Sprite>()->Init(TARGETPOSXINIT, 200.0f, 894 / 2, 1283 / 2, "asset\\texture\\youmu.png");

	GameObject* titleEnter = AddGameObject<GameObject>(3);			// 3�̓��C���ԍ�
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

	 // �V�����ǉ������ϐ�
	moveSpeed = 0.30f;
	targetPosX = 700.0f; // �����ɖڕW��X���W��ݒ�

	m_Transition = AddGameObject<Transition>(3);					// 3�̓��C���ԍ�
	m_Transition->FadeIn();
}

void Title::Update()
{
	// ��ʑJ�ڂ��I�����Ă��邩�m�F
	if (m_Transition->GetState() == Transition::State::Stop) {
		// �G���^�[�L�[�������ꂽ��t�F�[�h�A�E�g
		if (Input::GetKeyTrigger(VK_RETURN)) {
			m_Transition->FadeOut();
		}

		Sprite* sprite = titleChara->GetComponent<Sprite>();

		// 800����700�̊Ԃ͒ʏ�̑��x�ňړ�
		if (sprite->GetPosX() > 700 && sprite->GetPosX() <= 800) {
			sprite->SetPos(sprite->GetPosX() - moveSpeed, sprite->GetPosY());
		}
		else {
			// ����ȊO�͈̔͂ł͑��x��200�{�ɂȂ�A��ʂ̍��[�ɗ�����E�[�Ɉړ�
			sprite->SetPos(sprite->GetPosX() - (moveSpeed * 200.0f), sprite->GetPosY());

			// ��ʂ̍��[�ɗ����u��
			if (sprite->GetPosX() <= -100) {
				// �ʏ�̑��x�ɖ߂�
				moveSpeed = 0.30f;
				// �E�[�Ɉړ�
				sprite->SetPos(SCREEN_WIDTH, sprite->GetPosY());
			}
		}
	}
	// ��ʑJ�ڂ��I�����Ă邩�H
	if (m_Transition->GetState() == Transition::State::Finish)
	{
		Manager::SetScene<Game>();
	}
}