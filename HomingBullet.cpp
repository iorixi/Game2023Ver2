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
	BoundingSphereObj* boundingSphere = new BoundingSphereObj(1, m_Position);
	m_Child = AddChild<BoundingSphereObj>();
	m_Child = boundingSphere;

	//���Ԋ֌W������
	m_HomingPointUpdateTime = std::make_shared<ScheduledTask>(homingPointUpdateTime);
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

			//��莞�Ԍo�܂ŏ���
			if (m_HomingPointUpdateTime->GetFlg())
			{
				//�v���C���[�Ȃ�
				if (ownerChara == CHARACTER::PLAYER)
				{
					// ���̈ړ��������X�V
					// �G�Ɍ������x�N�g�����v�Z
					directionToEnemy = enemyPosition - m_Position;
					directionToEnemy.Normalize();
					// �O�̃t���[���̃x�N�g���Ƃ̊p�x���v�Z
					angleChange = oldDirection.Dot(directionToEnemy);
					//�x�N�g����ۑ�
					oldDirection = directionToEnemy;
				}
				//�G�Ȃ�
				else if (ownerChara == CHARACTER::ENEMY)
				{
					// ���̈ړ��������X�V
					// �v���C���[�Ɍ������x�N�g�����v�Z
					directionToPlayer = playerPosition - m_Position;
					directionToPlayer.Normalize();
					// �O�̃t���[���̃x�N�g���Ƃ̊p�x���v�Z
					angleChange = oldDirection.Dot(directionToPlayer);
					//�x�N�g����ۑ�
					oldDirection = directionToPlayer;
				}

				// �x�N�g���̊p�x��45�x�ȏ�ς�����ꍇ�͖����ɂ���
				if (angleChange <= cos(ToRadians(overAngle)))
				{
					isActive = false;
				}
				else
				{
					isActive = true;
				}
			}

			// ���̒ǔ����L���̏ꍇ�̂ݏ������s��
			if (isActive)
			{
				//�v���C���[�Ȃ�
				if (ownerChara == CHARACTER::PLAYER)
				{
					// �G�Ƌ��̋������v�Z
					float distanceToEnemy = Vector3::Distance(enemyPosition, m_Position);

					// �G�Ƃ̋��������ȉ��̏ꍇ�͒ǔ�����
					if (distanceToEnemy <= closeDistance)
					{
						// �����ɃX�s�[�h��������
						m_Velocity = directionToEnemy * speed;

						// ���̈ʒu���X�V
						m_Position += m_Velocity;
					}
					else
					{
						// �z�[�~���O�e����苗���ȏ�̓G�ɐڋ߂����ꍇ�A�ǔ��𖳌��ɂ��Ē��i����
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

					// �G�Ƃ̋��������ȉ��̏ꍇ�͒ǔ�����
					if (distanceToPlayer <= closeDistance)
					{
						// �����ɃX�s�[�h��������
						m_Velocity = directionToPlayer * speed;

						// ���̈ʒu���X�V
						m_Position += m_Velocity;
					}
					else
					{
						// �z�[�~���O�e����苗���ȏ�̓G�ɐڋ߂����ꍇ�A�ǔ��𖳌��ɂ��Ē��i����
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
				// �z�[�~���O�e����苗���ȏ�̓G�ɐڋ߂����ꍇ�A�ǔ��𖳌��ɂ��Ē��i����
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