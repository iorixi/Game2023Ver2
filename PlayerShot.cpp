#include "PlayerShot.h"
#include "input.h"
#include "player.h"
#include "scene.h"
#include "manager.h"
#include "camera.h"
#include "HomingBullet.h"

#include "CollisionUtils.h"
#include "HumanEnemy.h"
#include "enemy.h"
#include "score.h"

using namespace DirectX::SimpleMath;
using namespace Player;
using namespace Timer;
using namespace Enemy;

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
			HomingBullet* bullet = scene->AddGameObject<HomingBullet>(2);
			bullet->SetPosition(player->GetPosition() + Vector3(0.0f, 1.0f, 0.0f));
			bullet->SetVelocity(forward * 0.5f);
			addShotFlg = false;
		}
	}

	HumanObject* enemy = scene->GetGameObject<HumanObject>();

	std::vector<HumanObject*> enemyList = scene->GetGameObjects<HumanObject>();
	std::vector<HomingBullet*> bulletList = scene->GetGameObjects<HomingBullet>();

	//�G�ւ̓����蔻��
	for (HumanObject* enemy : enemyList)
	{
		//���ւ̓����蔻��
		for (HomingBullet* bullet : bulletList)
		{
			Vector3 enemyPosition = enemy->GetPosition();
			BoundingSphere* enemyHitSphere = enemy->GetEnemyHitSphere();
			BoundingSphere* bulletHitSphere = bullet->GetBulletHitSphere();

			//���̓����蔻��
			if (IsCollision(*enemyHitSphere, *bulletHitSphere))
			{
				Score* score = scene->GetGameObject<Score>();
				score->AddCount(1);

				bullet->SetDestroy();
			}
		}
	}
}

void Player::Shot::AddShot()
{
	addShotFlg = true;
}