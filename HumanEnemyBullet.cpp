#include "HumanEnemyBullet.h"
#include "player.h"
#include "scene.h"
#include "manager.h"
#include "camera.h"
#include "HomingBullet.h"

#include "CollisionUtils.h"
#include "HumanEnemy.h"
#include "enemy.h"
#include "score.h"
#include "CharaEnum.h"
#include "ScheduledTask.h"
#include "BoundingSphere.h"

using namespace DirectX::SimpleMath;
using namespace Player;
using namespace Timer;
using namespace Enemy;

void Enemy::Shot::Init()
{
	// m_ScheduledTask�̏�����
	m_ScheduledTask = std::make_unique<ScheduledTask>(0.2f);
	//�U���e���[�h�ɌŒ�
	enemyShootModo = ENEMYSHOOTMODO::HOMING;
}

void Enemy::Shot::Update()
{
	//���݂̃V�[�����擾
	Scene* scene = Manager::GetScene();
	//���݂̃V�[���̃v���C���[�̃I�u�W�F�N�g���擾
	PlayerObject* player = scene->GetGameObject<PlayerObject>();
	//���݂̃V�[���̓G�̃I�u�W�F�N�g���擾
	HumanObject* enemy = scene->GetGameObject<HumanObject>();

	//���݂̃V�[���̃J�������擾
	Camera* cameraobj = scene->GetGameObject<Camera>();

	Matrix viewmtx = cameraobj->GetViewMatrix();
	Vector3 ZAxis = Vector3(viewmtx._13, 0.0f, viewmtx._33);

	//�O�����x�N�g�����擾
	Vector3 forward = ZAxis;

	//�A�N�e�B�u��ԂȂ�
	if (enemy->GetIsActive())
	{
		if (m_ScheduledTask->GetFlg())
		{
			// �G�̂̌��݈ʒu�ɓG�̑O���x�N�g���������āA������ƑO�ɃI�t�Z�b�g�����ʒu���v�Z
			Vector3 enemySpawnShot = enemy->GetPosition() + forward * AddForwardEnemyShotSpawnPos;

			// �G�̑O���Ɍ������ăv���C���[�̈ʒu���擾
			Vector3 directionToEnemy = player->GetPosition() - enemySpawnShot;
			directionToEnemy.Normalize();

			// �e���쐬���A�G�l�~�[�̕����ɑ��x��ݒ�
			HomingBullet* bullet = scene->AddGameObject<HomingBullet>(2);
			//���͒N���ł��Ă��邩
			bullet->SetBulletOwner(CHARACTER::PLAYER);
			bullet->SetPosition(enemySpawnShot);
			bullet->SetVelocity(directionToEnemy * 0.5f);
			addShotFlg = false;
		}

		std::vector<HumanObject*> enemyList = scene->GetGameObjects<HumanObject>();
		std::vector<HomingBullet*> bulletList = scene->GetGameObjects<HomingBullet>();

		//���ւ̓����蔻��
		for (HomingBullet* bullet : bulletList)
		{
			Vector3 playerPosition = player->GetPosition();
			BoundingSphereObj* enemyHitSphere = player->GetPlayerHitSphere();
			BoundingSphereObj* bulletHitSphere = bullet->GetBulletHitSphere();

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

void Enemy::Shot::AddShot()
{
	addShotFlg = true;
}