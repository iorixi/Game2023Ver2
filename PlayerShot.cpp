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
#include "CharaEnum.h"
#include "BoundingSphere.h"
#include "ActionModo.h"

#include "audio.h"

using namespace DirectX::SimpleMath;
using namespace Player;
using namespace Timer;
using namespace Enemy;
using namespace Sound;

void Player::Shot::Init()
{
	// m_ScheduledTask�̏�����
	m_ScheduledTask = std::make_unique<ScheduledTask>(0.15f);
	//�U���e���[�h�ɌŒ�
	PlayerShootModo = PLAYERSHOOTMODO::HOMING;
}

void Player::Shot::Update()
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
	if (player->GetIsActive())
	{
		//�ړ���Ԃ������͉������ĂȂ��Ƃ��͋��𔭎˂���
		if (player->GetActionModo() == ActionModo::NONE || player->GetActionModo() == ActionModo::MOVE)
		{
			//�ʂ𐶐�
			if (m_ScheduledTask->GetFlg())
			{
				// �v���C���[�̌��݈ʒu�Ƀv���C���[�̑O���x�N�g���������āA������ƑO�ɃI�t�Z�b�g�����ʒu���v�Z
				Vector3 playerSpawnShot = player->GetPosition() + forward * AddForwardPlayerShotSpawnPos;
				playerSpawnShot.y += 1.8f;

				// �v���C���[�̑O���Ɍ������ăG�l�~�[�̈ʒu���擾
				Vector3 directionToEnemy = enemy->GetPosition() - playerSpawnShot;
				directionToEnemy.Normalize();

				// �e���쐬���A�G�l�~�[�̕����ɑ��x��ݒ�
				HomingBullet* bullet = scene->AddGameObject<HomingBullet>(2);
				//���͒N���ł��Ă��邩
				bullet->SetBulletOwner(CHARACTER::PLAYER);
				bullet->SetPosition(playerSpawnShot);
				bullet->SetVelocity(directionToEnemy * 0.5f);
				addShotFlg = false;
			}
		}

		std::vector<HumanObject*> enemyList = scene->GetGameObjects<HumanObject>();
		std::vector<HomingBullet*> bulletList = scene->GetGameObjects<HomingBullet>();

		//�G�ւ̓����蔻��
		for (HumanObject* enemy : enemyList)
		{
			//���ւ̓����蔻��
			for (HomingBullet* bullet : bulletList)
			{
				Vector3 enemyPosition = enemy->GetPosition();
				BoundingSphereObj* enemyHitSphere = enemy->GetEnemyHitSphere();
				BoundingSphereObj* bulletHitSphere = bullet->GetBulletHitSphere();

				if (bullet->GetBulletOwner() == CHARACTER::PLAYER)
				{
					//���̓����蔻��
					if (IsCollision(*enemyHitSphere, *bulletHitSphere))
					{
						//score->AddCount(1);
						Sound::Audio* m_SE = player->GetComponent<Audio>();
						m_SE->Play();

						enemy->Damege(1);
						bullet->SetDestroy();
					}
				}
			}
		}
	}
}

void Player::Shot::AddShot()
{
	addShotFlg = true;
}