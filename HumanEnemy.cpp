#include "manager.h"
#include "input.h"
#include "scene.h"
#include "HumanEnemy.h"
#include "shader.h"
#include "bullet.h"
#include "player.h"
#include "HomingBullet.h"
#include "CharaEnum.h"
#include "HumanEnemyBullet.h"
#include "BoundingSphere.h"
#include "ScheduledTask.h"
#include "Random.h"
#include "ActionModo.h"
#include "score.h"
#include <vector>

#include "newModel/CBoundingBox.h"
#include "newModel/CShader.h"
#include "newModel/CStaticMesh.h"
#include "newModel/CStaticMeshRenderer.h"
#include "newModel/CSphereMesh.h"
#include "newModel/dx11mathutil.h"

// �g�p����V�F�[�_�[
static CShader	g_shader;
// �X�^�e�B�b�N���b�V���i�W�I���g���f�[�^�j
static CStaticMesh g_staticmesh;
// ���b�V�������_���[
static CStaticMeshRenderer g_staticmeshrenderer;
// ��
static CSphereMesh g_sphere;// ���b�V�������_��
static CMeshRenderer g_meshrenderer;
// OBB
static CBoundingBox g_obb;

using namespace DirectX::SimpleMath;

// �G�̐l�ԃI�u�W�F�N�g�̏�����
void Enemy::HumanObject::Init()
{
	AddComponent<Shader>()->Load("shader\\vertexLightingOneSkinVS.cso", "shader\\vertexLightingPS.cso"); // �V�F�[�_�[�̃��[�h

	m_Position.y = 10; // Y���W�̏�����

	// ���f���̏�����
	{
		// �g�p����V�F�[�_�[�𐶐�
		g_shader.SetShader(
			"shader/vertexLightingVS.hlsl",   // ���_�V�F�[�_
			"shader/vertexLightingPS.hlsl");  // �s�N�Z���V�F�[�_

		// ���f���t�@�C����
		std::string filename[] = {
			"asset\\model\\furan\\furan2.pmx",
		};

		// ���b�V�������i�W�I���g���f�[�^�j
		g_staticmesh.Init(filename[0]);

		// �`��ׂ̈̃f�[�^����
		g_staticmeshrenderer.Init(g_staticmesh);

		// �}�e���A������
		MATERIAL mtrl;
		mtrl.Ambient = Color(0, 0, 0, 0);
		mtrl.Diffuse = Color(1, 1, 1, 0.3f);
		mtrl.Specular = Color(0, 0, 0, 0);
		mtrl.Shininess = 0;
		mtrl.Emission = Color(0, 0, 0, 0);
		mtrl.TextureEnable = false;

		g_material.Init(mtrl);
	}

	m_Scale = Vector3(0.15f, 0.15f, 0.15f); // �X�P�[���̏�����

	// �q�I�u�W�F�N�g�ɓ����蔻���ǉ�
	enemyHitSphere = std::make_shared<BoundingSphereObj>(0.1f, m_Position);

	// �ړ��؂�ւ��^�X�N�̏�����
	m_MoveChangeTask = std::make_unique<Timer::ScheduledTask>(4.0f);

	m_EnemyShot = AddComponent<Enemy::Shot>(); // �G�̃V���b�g�R���|�[�l���g�̒ǉ�
}

// �G�̐l�ԃI�u�W�F�N�g�̏I������
void Enemy::HumanObject::Uninit()
{
	GameObject::Uninit();
	g_staticmesh.Exit(); // ���b�V���̏I������
}

// �G�̐l�ԃI�u�W�F�N�g�̍X�V
void Enemy::HumanObject::Update()
{
	Scene* nowscene = Manager::GetScene(); // ���݂̃V�[���̎擾

	m_MoveTime += 1.0f / 60.0f; // �ړ����Ԃ̍X�V

	// �ړ��p�^�[���̐؂�ւ�
	if (cosf(m_MoveTime * 1.0f) * 0.1f > 0)
	{
		m_BlendRate += 0.1f;
		m_Frame++;
	}
	else if (cosf(m_MoveTime * 1.0f) * 0.1f < 0)
	{
		m_BlendRate += 0.1f;
		m_Frame--;
	}
	else
	{
		m_BlendRate -= 0.1f;
		m_Frame++;
	}

	// �v���C���[�I�u�W�F�N�g�̈ʒu���擾
	Player::PlayerObject* playerObject = nowscene->GetGameObject<Player::PlayerObject>();
	Vector3 playerPos = playerObject->GetPosition();
	Vector3 toPlayer;
	toPlayer.x = playerPos.x - m_Position.x;
	toPlayer.y = playerPos.y - m_Position.y;
	toPlayer.z = playerPos.z - m_Position.z;

	// �v���C���[�I�u�W�F�N�g����G�I�u�W�F�N�g�ւ̕����x�N�g�����v�Z
	toPlayer.Normalize();

	// Y�����̉�]�p�x���v�Z
	float yaw = atan2f(toPlayer.x, toPlayer.z);
	// X�����̉�]�p�x���v�Z
	float pitch = atan2f(toPlayer.y, sqrtf(toPlayer.x * toPlayer.x + toPlayer.z * toPlayer.z));

	// Z�����̉�]�p�x�i���̃T���v���ł͌Œ��0.0f�j
	float roll = 0.0f;

	// �ړ��p�^�[���̑I��
	if (m_MoveChangeTask->GetFlg())
	{
		C_Random random;
		actionModoSelect = random.Get_Random(1, 5);
		if (actionModoSelect == 5)
		{
			actionModoSelect = random.Get_Random(1, 5);
		}
	}

	// �ړ��p�^�[���ɂ��s��
	switch (actionModoSelect)
	{
	case 1:
		// �E�ړ�
		m_Position.x += 0.2f; // ��: 0.1f�P�ʂō��Ɉړ�
		actionModo = ActionModo::MOVE;
		break;
	case 2:
		// �E�ړ�
		m_Position.x += 0.2f; // ��: 0.1f�P�ʂō��Ɉړ�
		actionModo = ActionModo::MOVE;
		break;
	case 3:
		// ���ړ�
		m_Position.x -= 0.2f; // ��: 0.1f�P�ʂō��Ɉړ�
		actionModo = ActionModo::MOVE;
		break;
	case 4:
		// ���ړ�
		m_Position.x -= 0.2f; // ��: 0.1f�P�ʂō��Ɉړ�
		actionModo = ActionModo::MOVE;
		break;
	case 5:
		// �~�܂�
		actionModo = ActionModo::SPELL1;
		break;
	default:
		break;
	}

	// ��]��K�p
	m_Rotation = Vector3(pitch, yaw, roll);

	// �����蔻��
	enemyHitSphere->SetCenter(m_Position);
	std::vector<Score*> score = nowscene->GetGameObjects<Score>();
	score.at(0)->SetCount(hp);
}

// �G�̐l�ԃI�u�W�F�N�g�̕`��O����
void Enemy::HumanObject::PreDraw()
{
	// �}�e���A������
	// �f�o�C�X�R���e�L�X�g�擾
	ID3D11DeviceContext* devicecontext;
	devicecontext = Renderer::GetDeviceContext();

	// ���[���h�ϊ��s�񐶐�
	Matrix mtx;
	DX11MakeWorldMatrixRadian(
		mtx,
		m_Scale,                            // �X�P�[��
		m_Rotation,                         // �p��
		m_Position);                        // �ʒu

	// GPU�ɍs����Z�b�g����
	Renderer::SetWorldMatrix(&mtx);

	// �V�F�[�_�[��GPU�ɃZ�b�g
	g_shader.SetGPU();

	// ���f���`��
	g_staticmeshrenderer.Draw();

	// ���E�{�b�N�X�`��
	mtx = g_obb.MakeWorldMtx(m_Scale, mtx);

	Renderer::SetWorldMatrix(&mtx);
	g_material.SetGPU();
	g_meshrenderer.Draw();
}

// �A�N�e�B�u��Ԃ�ݒ肷��
void Enemy::HumanObject::SetIsActive(bool _isActive)
{
	isActive = _isActive;
}

// �A�N�e�B�u��Ԃ��擾����
bool Enemy::HumanObject::GetIsActive()
{
	return isActive;
}

// �A�N�V�������[�h���擾����
ActionModo Enemy::HumanObject::GetActionModo()
{
	return actionModo;
}

// �A�N�V�������[�h��ݒ肷��
void Enemy::HumanObject::SetActionModo(ActionModo actionmodo)
{
	actionModo = actionmodo;
}