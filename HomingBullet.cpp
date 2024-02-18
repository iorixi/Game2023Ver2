#include "HomingBullet.h"
#include "main.h"
#include "scene.h"
#include "manager.h"
#include "modelRenderer.h"
#include "enemy.h"
#include "shadow.h"
#include "shader.h"
#include "explosion.h"
#include "score.h"
#include "BoundingSphere.h"

#include "HumanEnemy.h"
#include "player.h"
#include "CharaEnum.h"
#include "camera.h"
#include "ScheduledTask.h"
#include "ToRadians.h"
#include "CharaEnum.h"
#include <iostream>
#include "CalculateAngle.h"

using namespace DirectX::SimpleMath;
using namespace Player;
using namespace Enemy;
using namespace Timer;

HomingBullet::HomingBullet()
{
	ownerChara = CHARACTER::NONE;
}

HomingBullet::HomingBullet(enum class CHARACTER chara)
{
	ownerChara = chara;
}

HomingBullet::~HomingBullet()
{
}

void HomingBullet::Load()
{
	ModelRenderer::Preload("asset\\model\\bullet.obj");
}

void HomingBullet::Unload()
{
}

void HomingBullet::Init()
{
	AddComponent<Shader>()->Load("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");
	AddComponent<ModelRenderer>()->Load("asset\\model\\bullet.obj");

	AddComponent<Shadow>()->SetSize(0.5f);

	//�q�I�u�W�F�N�g�ɓ����蔻���ǉ�
	m_Child = std::make_unique<BoundingSphereObj>(1, m_Position);

	//���Ԋ֌W������
	m_HomingPointUpdateTime = std::make_shared<ScheduledTask>(homingPointUpdateTime);
	m_HomingTimeDestroy = std::make_shared<ScheduledTask>();
}

void HomingBullet::Update()
{
	try
	{
		//�I�[�i�[���ݒ肳��Ă��邩
		if (ownerChara != CHARACTER::NONE)
		{
			// ���݂̃V�[�����擾
			Scene* currentScene = Manager::GetScene();
			// ���݂̃V�[���̃v���C���[�̃I�u�W�F�N�g���擾
			PlayerObject* player = currentScene->GetGameObject<PlayerObject>();
			// ���݂̃V�[���̓G�̃I�u�W�F�N�g���擾
			HumanObject* enemy = currentScene->GetGameObject<HumanObject>();

			// �v���C���[�̍��W���擾
			Vector3 playerPosition = player->GetPosition();
			Vector3 enemyPosition = enemy->GetPosition();
			float angleChange = 0.0f;

			// ���̒ǔ����L���̏ꍇ�̂ݏ������s��
			if (isActive)
			{
				//��莞�Ԃ������珈��
				//�ǔ��ʒu�X�V
				if (m_HomingPointUpdateTime->GetFlg())
				{
					//�v���C���[�Ȃ�
					if (ownerChara == CHARACTER::PLAYER)
					{
						// ���̈ړ��������X�V
						// �G�Ɍ������x�N�g�����v�Z
						directionToEnemy = enemyPosition - m_Position;
						directionToEnemy.y += 1.0f;

						directionToEnemy.Normalize();
						// �O�̃t���[���̃x�N�g���Ƃ̊p�x���v�Z
						angleChange = CalculateAngle(oldDirection, directionToEnemy);
						//�x�N�g����ۑ�
						oldDirection = directionToEnemy;
					}
					//�G�Ȃ�
					else if (ownerChara == CHARACTER::ENEMY)
					{
						// ���̈ړ��������X�V
						// �v���C���[�Ɍ������x�N�g�����v�Z
						directionToPlayer = playerPosition - m_Position;
						directionToPlayer.y += 1.0f;
						directionToPlayer.Normalize();
						// �O�̃t���[���̃x�N�g���Ƃ̊p�x���v�Z
						angleChange = CalculateAngle(oldDirection, directionToPlayer);
						//�x�N�g����ۑ�
						oldDirection = directionToPlayer;
					}

					// �x�N�g���̊p�x�����p�x�ȏ�ς�����ꍇ�͖����ɂ���
					if (angleChange >= ToRadians(overAngle)) {
						isActive = false;
					}
				}

				//�v���C���[�Ȃ�
				if (ownerChara == CHARACTER::PLAYER)
				{
					// �G�Ƌ��̋������v�Z
					float distanceToEnemy = Vector3::Distance(enemyPosition, m_Position);

					// �G�Ƃ̋��������ȏ�̏ꍇ�͒ǔ�����
					if (distanceToEnemy >= closeDistance)
					{
						// �����ɃX�s�[�h��������
						m_Velocity = directionToEnemy * speed;

						// ���̈ʒu���X�V
						m_Position += m_Velocity;
					}
					else
					{
						// �z�[�~���O�e����苗���ȉ��̓G�ɐڋ߂����ꍇ�A�ǔ��𖳌��ɂ��Ē��i����
						isActive = false;
						Vector3 forward = m_Velocity;
						forward.Normalize();

						// �z�[�~���O�e�����i���������ݒ肷��
						m_Velocity = forward * speed; // forward�̓z�[�~���O�e�̑O�����x�N�g����\�����̂Ƃ��܂�

						// ���̈ʒu���X�V
						m_Position += m_Velocity;
					}
				}
				else if (ownerChara == CHARACTER::ENEMY)
				{
					// �G�Ƌ��̋������v�Z
					float distanceToPlayer = Vector3::Distance(playerPosition, m_Position);

					// �G�Ƃ̋��������ȏ�̏ꍇ�͒ǔ�����
					if (distanceToPlayer >= closeDistance)
					{
						// �����ɃX�s�[�h��������
						m_Velocity = directionToPlayer * speed;

						// ���̈ʒu���X�V
						m_Position += m_Velocity;
					}
					else
					{
						// �z�[�~���O�e����苗���ȉ��̓G�ɐڋ߂����ꍇ�A�ǔ��𖳌��ɂ��Ē��i����
						isActive = false;
						Vector3 forward = m_Velocity;
						forward.Normalize();

						// �z�[�~���O�e�����i���������ݒ肷��
						m_Velocity = forward * speed; // forward�̓z�[�~���O�e�̑O�����x�N�g����\�����̂Ƃ��܂�

						// ���̈ʒu���X�V
						m_Position += m_Velocity;
					}
				}
			}
			else
			{
				//�ǔ�����
				isActive = false;
				Vector3 forward = m_Velocity;
				forward.Normalize();
				// �z�[�~���O�e�����i���������ݒ肷��
				m_Velocity = forward * speed; // forward�̓z�[�~���O�e�̑O�����x�N�g����\�����̂Ƃ��܂�

				// ���̈ʒu���X�V
				m_Position += m_Velocity;
			}

			// ���̈ʒu�����͈͂𒴂����ꍇ�͍폜
			if (m_Position.Length() > 100.0f)
			{
				SetDestroy();
			}

			//���̏��Ŏ��Ԃ��Z�b�g
			if (m_timerSet == false)
			{
				m_HomingTimeDestroy->SetTimer(m_timer);
				m_timerSet = true;
			}
			// �w�莞�Ԍo�߂ŋ�����
			if (m_HomingTimeDestroy->GetFlg())
			{
				SetDestroy();
			}

			// �����蔻��̈ʒu���X�V
			m_Child->SetCenter(m_Position);
		}
		else
		{
			throw std::exception("�I�[�i�[�L�����N�^�[���ݒ肳��Ă��܂���B");
		}
	}
	catch (const std::exception& e)
	{
		// ��O���L���b�`���A�K�؂ȏ������s���܂�
		std::cerr << "HomingBullet::Update �ŗ�O���������܂���: " << e.what() << std::endl;
	}
}

void HomingBullet::SetBulletOwner(enum class CHARACTER chara)
{
	ownerChara = chara;
}

CHARACTER HomingBullet::GetBulletOwner()
{
	return ownerChara;
}