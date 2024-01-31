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

using namespace DirectX::SimpleMath;
using namespace Player;
using namespace Enemy;
using namespace Timer;

HomingBullet::HomingBullet()
{
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
	BoundingSphere* boundingSphere = new BoundingSphere(1, m_Position);
	m_Child = AddChild<BoundingSphere>();
	m_Child = boundingSphere;

	//���Ԋ֌W������
	m_HomingPointUpdateTime = std::make_shared<ScheduledTask>(homingPointUpdateTime);
}

void HomingBullet::Update()
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

	//��莞�Ԍo�܂ŏ���
	if (m_HomingPointUpdateTime->GetFlg())
	{
		// ���̈ړ��������X�V
		// �G�Ɍ������x�N�g�����v�Z
		directionToEnemy = enemyPosition - m_Position;
		directionToEnemy.Normalize();

		// �O�̃t���[���̃x�N�g���Ƃ̊p�x���v�Z
		float angleChange = oldDirection.Dot(directionToEnemy);
		// �x�N�g���̊p�x��45�x�ȏ�ς�����ꍇ�͖����ɂ���
		if (angleChange <= cos(ToRadians(overAngle)))
		{
			isActive = false;
		}
		else
		{
			isActive = true;
		}

		oldDirection = directionToEnemy;
	}

	// ���̒ǔ����L���̏ꍇ�̂ݏ������s��
	if (isActive)
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