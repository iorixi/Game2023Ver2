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
#include "PlayerShootModo.h"
#include "CircleSkillShoot.h"
#include <array>

using namespace DirectX::SimpleMath;

void Player::Shot::Init()
{
	// m_ScheduledTask�̏�����
	m_ScheduledTask = std::make_unique<Timer::ScheduledTask>(0.15f);
	m_ShootSkill = std::make_unique<Timer::ScheduledTask>();
	m_ShootSkillCool = std::make_unique<Timer::ScheduledTask>();
	m_ShootSkillRate = std::make_unique<Timer::ScheduledTask>();

	//�U���e���[�h�ɌŒ�
	playerShootModo = PlayerShootModo::HOMING;
}

void Player::Shot::Update()
{
	//���݂̃V�[�����擾
	Scene* scene = Manager::GetScene();
	//���݂̃V�[���̃v���C���[�̃I�u�W�F�N�g���擾
	Player::PlayerObject* player = scene->GetGameObject<Player::PlayerObject>();
	//���݂̃V�[���̓G�̃I�u�W�F�N�g���擾
	Enemy::HumanObject* enemy = scene->GetGameObject<Enemy::HumanObject>();

	//���݂̃V�[���̃J�������擾
	Camera* cameraobj = scene->GetGameObject<Camera>();

	Matrix viewmtx = cameraobj->GetViewMatrix();
	Vector3 ZAxis = Vector3(viewmtx._13, 0.0f, viewmtx._33);

	//�O�����x�N�g�����擾
	Vector3 forward = ZAxis;

	//�A�N�e�B�u��ԂȂ�
	if (player->GetIsActive())
	{
		//�X�y���P����
		if (Input::GetKeyTrigger('Q' || VK_LBUTTON || VK_UP))
		{
			if (m_ShootFlg)
			{
				player->SetActionModo(ActionModo::SPELL1);
				m_ShootSkill->SetTimer(0.5f);
				m_ShootFlg = false;

				// �v���C���[�̌��݈ʒu�Ƀv���C���[�̑O���x�N�g���������āA������ƑO�ɃI�t�Z�b�g�����ʒu���v�Z
				Vector3 playerSpawnShot = player->GetPosition();

				// �e���쐬���A�G�l�~�[�̕����ɑ��x��ݒ�
				std::array<CircleSkillShoot*, 5> bullets;
				int Defdegree = 3;
				float slidePosx = 5.0f;
				for (auto i = 0; i < bullets.size(); i++)
				{
					Vector3 newPlayerSpawnShot = playerSpawnShot;

					newPlayerSpawnShot.x + slidePosx * -2 + slidePosx * i;

					bullets.at(i) = scene->AddGameObject<CircleSkillShoot>(2);
					bullets.at(i)->SetBulletOwner(CHARACTER::PLAYER);
					bullets.at(i)->SetPosition(playerSpawnShot);
					bullets.at(i)->SetBulletDegree(Defdegree * -2 + Defdegree * i);
				}
			}
		}

		if (m_ShootSkill->GetFlg())
		{
			player->SetActionModo(ActionModo::NONE);
			m_ShootFlg = true;
		}

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
	}
}

void Player::Shot::AddShot()
{
	addShotFlg = true;
}