#include "PlayerShot.h"
#include "input.h"
#include "player.h"
#include "scene.h"
#include "manager.h"
#include "camera.h"
#include "bullet.h"
#include "ScheduledTask.h"

using namespace DirectX::SimpleMath;
using namespace Player;
using namespace Timer;

void Player::Shot::Init()
{
	// m_ScheduledTask�̏�����
	m_ScheduledTask = std::make_unique<ScheduledTask>(0.3f);
}

void Player::Shot::Update()
{
	//���݂̃V�[�����擾
	Scene* scene = Manager::GetScene();
	//���݂̃V�[���̃v���C���[�̃I�u�W�F�N�g���擾
	PlayerObject* player = scene->GetGameObject<PlayerObject>();

	//���݂̃V�[���̃J�������擾
	Camera* cameraobj = scene->GetGameObject<Camera>();

	Matrix viewmtx = cameraobj->GetViewMatrix();
	Vector3 ZAxis = Vector3(viewmtx._13, 0.0f, viewmtx._33);

	//�O�����x�N�g�����擾
	Vector3 forward = ZAxis;

	//�e����
	if (Input::GetKeyPress(VK_LBUTTON))
	{
		if (m_ScheduledTask->GetFlg())
		{
			Bullet* bullet = scene->AddGameObject<Bullet>(2);
			bullet->SetPosition(player->GetPosition() + Vector3(0.0f, 1.0f, 0.0f));
			bullet->SetVelocity(forward * 0.5f);
			addShotFlg = false;
		}
	}
}

void Player::Shot::AddShot()
{
	addShotFlg = true;
}