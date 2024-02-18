#include "CircleSkillShoot.h"
#include "scene.h"
#include "manager.h"
#include "modelRenderer.h"
#include "enemy.h"
#include "shadow.h"
#include "shader.h"
#include "score.h"
#include "BoundingSphere.h"
#include "HumanEnemy.h"
#include "player.h"
#include "CharaEnum.h"
#include "camera.h"
#include "ScheduledTask.h"
#include "CharaEnum.h"
#include <iostream>

using namespace DirectX::SimpleMath;

CircleSkillShoot::CircleSkillShoot()
{
}

CircleSkillShoot::CircleSkillShoot(CHARACTER chara)
{
	ownerChara = chara;
	if (ownerChara == CHARACTER::PLAYER)
	{
		// ���݂̃V�[�����擾
		Scene* currentScene = Manager::GetScene();
		// ���݂̃V�[���̃v���C���[�̃I�u�W�F�N�g���擾
		Player::PlayerObject* player = currentScene->GetGameObject<Player::PlayerObject>();

		m_PositionStart = player->GetPosition();
	}
	else if (ownerChara == CHARACTER::ENEMY)
	{
		// ���݂̃V�[�����擾
		Scene* currentScene = Manager::GetScene();
		// ���݂̃V�[���̓G�̃I�u�W�F�N�g���擾
		Enemy::HumanObject* enemy = currentScene->GetGameObject<Enemy::HumanObject>();

		m_PositionStart = enemy->GetPosition();
	}
}

CircleSkillShoot::~CircleSkillShoot()
{
}

void CircleSkillShoot::Load()
{
	ModelRenderer::Preload("asset\\model\\bullet.obj");
}

void CircleSkillShoot::Unload()
{
}

void CircleSkillShoot::Init()
{
	AddComponent<Shader>()->Load("shader\\PlayerShootBulletVS.cso", "shader\\EnemyShootBulletVS.cso", "shader\\vertexLightingPS.cso");
	AddComponent<ModelRenderer>()->Load("asset\\model\\bullet.obj");

	AddComponent<Shadow>()->SetSize(0.5f);

	m_Position.y += 2.0f;

	//�q�I�u�W�F�N�g�ɓ����蔻���ǉ�
	m_Child = std::make_unique<BoundingSphereObj>(1, m_Position);

	//���Ԋ֌W������
	m_ShootTime = std::make_unique<Timer::ScheduledTask>(0.5f);
}
void CircleSkillShoot::Update()
{
	try
	{
		if (ownerChara == CHARACTER::ENEMY)
		{
			Shader* shader = GetComponent<Shader>();

			shader->SetPlayerShder(false);
		}

		if (ownerChara == CHARACTER::PLAYER)
		{
			// ���݂̃V�[�����擾
			Scene* currentScene = Manager::GetScene();
			// ���݂̃V�[���̃v���C���[�̃I�u�W�F�N�g���擾
			Player::PlayerObject* player = currentScene->GetGameObject<Player::PlayerObject>();
			// ���݂̃V�[���̓G�̃I�u�W�F�N�g���擾
			Enemy::HumanObject* enemy = currentScene->GetGameObject<Enemy::HumanObject>();

			if (m_ShootTime->GetFlg())
			{
				m_ShootMoveFlg = true;
			}

			if (!m_ShootMoveFlg)
			{
				// �v���C���[�̈ʒu���擾
				Vector3 playerPosition = player->GetPosition();

				// �ʂ��v���C���[�̉E���ɔz�u���邽�߂̃I�t�Z�b�g��ݒ�
				float offsetX = 1.0f; // ���̃I�t�Z�b�g�l

				// �ʂ̈ʒu��ݒ�
				m_Position = playerPosition + Vector3::Right * offsetX;
				m_Position.y += 2.0f;
			}
			else
			{
				if (!m_EnemyPositionSaved)
				{
					// �G�̈ʒu���擾
					m_EnemyPosition = enemy->GetPosition();
					m_EnemyPositionSaved = true;
				}

				// �v���C���[�̈ʒu���擾
				Vector3 playerPosition = player->GetPosition();

				// �ʂ��G�̈ʒu�Ɍ������������v�Z
				Vector3 directionToEnemy = m_EnemyPosition - m_Position;
				directionToEnemy.Normalize();

				// �v���C���[�ƓG�̊Ԃ̃x�N�g�����v�Z
				Vector3 directionToPlayer = m_EnemyPosition - playerPosition;
				directionToPlayer.Normalize();

				// �v���C���[�ƓG�̊Ԃ̃x�N�g����y�����ɓx������]������
				Matrix rotation = Matrix::CreateRotationY(DirectX::XMConvertToRadians(Degree));
				Vector3 rotatedDirection = Vector3::Transform(directionToPlayer, rotation);

				// �ʂ𔭎˂��������ݒ�
				Vector3 shootDirection = rotatedDirection;

				// �ʂ𔭎˂�������Ɉړ�������
				m_Velocity = shootDirection * speed;
				m_Position += m_Velocity;
			}
		}
		else
		{
			//�G�̏ꍇ

			// ���݂̃V�[�����擾
			Scene* currentScene = Manager::GetScene();
			// ���݂̃V�[���̃v���C���[�̃I�u�W�F�N�g���擾
			Player::PlayerObject* player = currentScene->GetGameObject<Player::PlayerObject>();
			// ���݂̃V�[���̓G�̃I�u�W�F�N�g���擾
			Enemy::HumanObject* enemy = currentScene->GetGameObject<Enemy::HumanObject>();

			if (m_ShootTime->GetFlg())
			{
				m_ShootMoveFlg = true;
			}

			if (!m_ShootMoveFlg)
			{
				// �G�̈ʒu���擾
				Vector3 enemyPosition = enemy->GetPosition();

				// �ʂ��v���C���[�̉E���ɔz�u���邽�߂̃I�t�Z�b�g��ݒ�
				float offsetX = 1.0f; // ���̃I�t�Z�b�g�l

				// �ʂ̈ʒu��ݒ�
				m_Position = enemyPosition + Vector3::Right * offsetX;
				m_Position.y += 2.0f;
			}
			else
			{
				if (!m_EnemyPositionSaved)
				{
					// �G�̈ʒu���擾
					m_EnemyPosition = enemy->GetPosition();
					m_EnemyPositionSaved = true;
				}

				// �G�̈ʒu���擾
				Vector3 enemyPosition = enemy->GetPosition();

				// �ʂ��G�̈ʒu�Ɍ������������v�Z
				Vector3 directionToEnemy = m_Position - m_EnemyPosition;
				directionToEnemy.Normalize();

				// �v���C���[�ƓG�̊Ԃ̃x�N�g�����v�Z
				Vector3 directionToPlayer = player->GetPosition() - m_EnemyPosition;
				directionToPlayer.Normalize();

				// �v���C���[�ƓG�̊Ԃ̃x�N�g����y�����ɓx������]������
				Matrix rotation = Matrix::CreateRotationY(DirectX::XMConvertToRadians(Degree));
				Vector3 rotatedDirection = Vector3::Transform(directionToPlayer, rotation);

				// �ʂ𔭎˂��������ݒ�
				Vector3 shootDirection = rotatedDirection;

				// �ʂ𔭎˂�������Ɉړ�������
				m_Velocity = shootDirection * speed;
				m_Position += m_Velocity;
			}
		}

		// ���̈ʒu�����͈͂𒴂����ꍇ�͍폜
		if ((m_Position - m_PositionStart).Length() > 300.0f)
		{
			SetDestroy();
		}

		// �����蔻��̈ʒu���X�V
		Vector3 newpos = m_Position;
		newpos.y -= 2.0f;
		m_Child->SetCenter(newpos);
	}
	catch (const std::exception& e)
	{
		// ��O���L���b�`���A�G���[���b�Z�[�W���o��
		std::cerr << "CircleSkillShoot::Update: " << e.what() << std::endl;
	}
}

void CircleSkillShoot::SetBulletOwner(enum class CHARACTER chara)
{
	ownerChara = chara;

	if (ownerChara == CHARACTER::PLAYER)
	{
		// ���݂̃V�[�����擾
		Scene* currentScene = Manager::GetScene();
		// ���݂̃V�[���̃v���C���[�̃I�u�W�F�N�g���擾
		Player::PlayerObject* player = currentScene->GetGameObject<Player::PlayerObject>();

		m_PositionStart = player->GetPosition();
	}
	else if (ownerChara == CHARACTER::ENEMY)
	{
		// ���݂̃V�[�����擾
		Scene* currentScene = Manager::GetScene();
		// ���݂̃V�[���̓G�̃I�u�W�F�N�g���擾
		Enemy::HumanObject* enemy = currentScene->GetGameObject<Enemy::HumanObject>();

		m_PositionStart = enemy->GetPosition();
	}
}

void CircleSkillShoot::SetBulletDegree(float degree)
{
	Degree = degree;
}

CHARACTER CircleSkillShoot::GetBulletOwner()
{
	return ownerChara;
}