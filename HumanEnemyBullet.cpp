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
#include "CircleSkillShoot.h"
#include "audio.h"

using namespace DirectX::SimpleMath;

void Enemy::Shot::Init()
{
	// m_ScheduledTask�̏�����
	m_ScheduledTask = std::make_unique<Timer::ScheduledTask>(0.2f);
	m_SpellBulletRate = std::make_unique<Timer::ScheduledTask>(0.7f);
	//�U���e���[�h�ɌŒ�
	enemyShootModo = ENEMYSHOOTMODO::HOMING;
}

void Enemy::Shot::Update()
{
	//���݂̃V�[�����擾
	Scene* scene = Manager::GetScene();
	//���݂̃V�[���̃v���C���[�̃I�u�W�F�N�g���擾
	Player::PlayerObject* player = scene->GetGameObject<Player::PlayerObject>();
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
			// �G�̌��݈ʒu�ɓG�̑O���x�N�g���������āA������ƑO�ɃI�t�Z�b�g�����ʒu���v�Z
			Vector3 enemySpawnShot = enemy->GetPosition() + forward * AddForwardEnemyShotSpawnPos;
			enemySpawnShot.y += 1.6f;
			// �G�̑O���Ɍ������ăv���C���[�̈ʒu���擾
			Vector3 directionToEnemy = player->GetPosition() - enemySpawnShot;
			directionToEnemy.Normalize();

			// �e���쐬���A�G�l�~�[�̕����ɑ��x��ݒ�
			HomingBullet* bullet = scene->AddGameObject<HomingBullet>(2);
			//���͒N���ł��Ă��邩
			bullet->SetBulletOwner(CHARACTER::ENEMY);
			bullet->SetPosition(enemySpawnShot);
			bullet->SetVelocity(directionToEnemy * 0.5f);
			addShotFlg = false;
		}

		if (m_SpellBulletRate->GetFlg())
		{
			// �v���C���[�̌��݈ʒu�Ƀv���C���[�̑O���x�N�g���������āA������ƑO�ɃI�t�Z�b�g�����ʒu���v�Z
			Vector3 enemySpawnShot = enemy->GetPosition();

			// �e���쐬���A�G�l�~�[�̕����ɑ��x��ݒ�
			std::array<CircleSkillShoot*, 5> bullets;
			int Defdegree = 3;
			float slidePosx = 5.0f;
			for (auto i = 0; i < bullets.size(); i++)
			{
				Vector3 newEnemySpawnShot = enemySpawnShot;

				newEnemySpawnShot.x + slidePosx * -2 + slidePosx * i;

				bullets.at(i) = scene->AddGameObject<CircleSkillShoot>(2);
				bullets.at(i)->SetBulletOwner(CHARACTER::ENEMY);
				bullets.at(i)->SetPosition(enemySpawnShot);
				bullets.at(i)->SetBulletDegree(Defdegree * -2 + Defdegree * i);
			}
		}

		std::vector<HumanObject*> enemyList = scene->GetGameObjects<HumanObject>();
		std::vector<HomingBullet*> bulletList = scene->GetGameObjects<HomingBullet>();

		//���ւ̓����蔻��
		for (HomingBullet* bullet : bulletList)
		{
			Vector3 playerPosition = player->GetPosition();
			BoundingSphereObj* playerHitSphere = player->GetPlayerHitSphere();
			BoundingSphereObj* bulletHitSphere = bullet->GetBulletHitSphere();

			if (bullet->GetBulletOwner() == CHARACTER::ENEMY)
			{
				//���̓����蔻��
				if (IsCollision(*playerHitSphere, *bulletHitSphere))
				{
					Score* score = scene->GetGameObject<Score>();
					Sound::Audio* m_SE = player->GetComponent <Sound::Audio >();
					m_SE->Play();
					player->Damege(1);
					bullet->SetDestroy();
				}
			}
		}

		//CircleSkillShoot�̓����蔻��

		std::vector<CircleSkillShoot*> skillBulletList = scene->GetGameObjects<CircleSkillShoot>();

		//���ւ̓����蔻��
		for (CircleSkillShoot* bullet : skillBulletList)
		{
			Vector3 playerPosition = player->GetPosition();
			BoundingSphereObj* playerHitSphere = player->GetPlayerHitSphere();
			BoundingSphereObj* bulletHitSphere = bullet->GetBulletHitSphere();

			//�I�[�i�[���v���C���[�Ȃ瓖���蔻��
			if (bullet->GetBulletOwner() == CHARACTER::ENEMY)
			{
				//���̓����蔻��
				if (IsCollision(*playerHitSphere, *bulletHitSphere))
				{
					Score* score = scene->GetGameObject<Score>();
					Sound::Audio* m_SE = enemy->GetComponent <Sound::Audio >();
					m_SE->Play();
					player->Damege(3);
					bullet->SetDestroy();
				}
			}
		}
	}
}

void Enemy::Shot::AddShot()
{
	addShotFlg = true;
}