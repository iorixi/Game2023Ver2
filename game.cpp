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

using namespace DirectX::SimpleMath;
using namespace Player;

// �Q�[����������
void Game::Init()
{
	AddGameObject<Camera>(0);
	AddGameObject<Sky>(1);
	AddGameObject<Field>(1);
	AddGameObject<PlayerObject>(1);
	AddGameObject<Enemy>(1);
	AddGameObject<Score>(3);

	// ��ʑJ�ڃI�u�W�F�N�g��o�^
	m_Transition = AddGameObject<Transition>(3);
	m_Transition->FadeIn();
}

// �Q�[���I������
void Game::Uninit()
{
}

// �Q�[���X�V����
void Game::Update()
{
	// �t�F�[�h�A�E�g���I�����Ă��邩�H
	if (m_Transition->GetState() == Transition::State::Finish)
	{
		Manager::SetScene<Result>();
	}
}
