#include "manager.h"
#include "renderer.h"
#include "shader.h"
#include "Player.h"
#include "input.h"
#include "box.h"
#include "audio.h"
#include "bullet.h"
#include "shadow.h"
#include "camera.h"
#include "animationModel.h"
#include "field.h"
#include "PlayerMove.h"
#include "PlayerFloating.h"
#include "PlayerEvasive.h"
#include "PlayerShot.h"
#include "HumanEnemy.h"
#include "BoundingSphere.h"
#include "ActionModo.h"
#include "score.h"
#include "PlayerBullerHit.h"

#include <vector>

#include "newModel/CBoxMesh.h"
#include "newModel/CBoundingBox.h"
#include "newModel/CMaterial.h"
#include "newModel/CShader.h"
#include "newModel/CStaticMesh.h"
#include "newModel/CStaticMeshRenderer.h"
#include "newModel/CBoundingSphere.h"
#include "newModel/CSphereMesh.h"
#include "newModel/dx11mathutil.h"

using namespace DirectX::SimpleMath;

//�v���C���[�̈ړ����x
const float moveSpeed = 1.5f;

// �`��ׂ̈ɕK�v�ȏ��
// �g�p����V�F�[�_�[
static CShader	g_shader;

// �X�^�e�B�b�N���b�V���i�W�I���g���f�[�^�j
static CStaticMesh g_staticmesh;

// ���b�V�������_���[
static CStaticMeshRenderer g_staticmeshrenderer;

static Vector3	g_move = { 0.0f,0.0f,0.0f };			// �ړ���
static Vector3	g_destrot = { 0.0f,0.0f,0.0f };			// �ڕW��]�p�x

// ��
static CSphereMesh g_sphere;// ���b�V�������_��
static CMeshRenderer g_meshrenderer;

// �{�b�N�X���b�V��
static CBoxMesh	g_box;

// OBB
static CBoundingBox g_obb;

// BS
static CBoundingSphere g_bs;

void Player::PlayerObject::Init()
{
	AddComponent<Shader>()->Load("shader\\vertexLightingOneSkinVS.cso", "shader\\vertexLightingPS.cso");

	m_Model = AddComponent<AnimationModel>();

	m_Model->Load("asset\\model\\KachujinGRosales.fbx");

	//�}�e���A������
	{
		// �g�p����V�F�[�_�[�𐶐�
		g_shader.SetShader(
			"shader/vertexLightingVS.hlsl",					// ���_�V�F�[�_
			"shader/vertexLightingPS.hlsl");				// �s�N�Z���V�F�[�_

		// ���f���t�@�C����
		//�ǂݍ��݂��܂������Ȃ�������u8�݂����ȏ������T��
		std::string filename[] = {
			"asset\\model\\youmu\\youmu2.pmx",
			"asset\\model\\Akai_Run.fbx",
		};

		// ���b�V�������i�W�I���g���f�[�^�j
		g_staticmesh.Init(filename[0]);

		//// �`��ׂ̈̃f�[�^����
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

	AddComponent<Shadow>()->SetSize(1.5f);

	m_SE = AddComponent<Sound::Audio>();
	m_SE->Load("asset\\audio\\damage.wav");

	m_Scale = Vector3(0.15f, 0.15f, 0.15f);

	m_Delay = AddComponent<Timer::DelayCompnent>();
	m_Delay->SetLoop(true);
	m_Delay->SetdelayTime(0.2f);

	//�v���C���[�̃R���|�[�l���g
	m_PlayerMove = AddComponent<Player::Move>();
	m_PlayerFloating = AddComponent<Player::Floating>();
	m_PlayerEvasive = AddComponent<Player::Evasive>();
	m_PlayerShot = AddComponent<Player::Shot>();
	m_BulletHit = AddComponent<Player::BulletHit>();

	m_Position.y += 10;
	m_Position.x += 40;

	//�q�I�u�W�F�N�g�ɓ����蔻���ǉ�
	//�����蔻��
	playerHitSphere = std::make_shared<BoundingSphereObj>(0.2f, m_Position);
	actionModo = ActionModo::NONE;
}

void Player::PlayerObject::Update()
{
	Vector3 oldPosition = m_Position;

	Scene* nowscene = Manager::GetScene();
	Camera* cameraobj = nowscene->GetGameObject<Camera>();

	Matrix viewmtx = cameraobj->GetViewMatrix();
	Vector3 ZAxis = Vector3(viewmtx._13, 0.0f, viewmtx._33);
	Vector3 XAxis = Vector3(viewmtx._11, 0.0f, viewmtx._31);

	//�O�����x�N�g�����擾
	Vector3 forward = ZAxis;

	// �t�B�[���h�I�u�W�F�N�g�擾
	Field* fieldobj = nowscene->GetGameObject<Field>();

	//�@�͈̓`�F�b�N
	Vector3 max = fieldobj->GetMax();
	Vector3 min = fieldobj->GetMin();

	actionModo = ActionModo::NONE;

	if (Input::GetKeyPress('W'))
	{
		m_BlendRate += 0.1f;
		m_Frame++;
	}
	else if (Input::GetKeyPress('S'))
	{
		m_BlendRate += 0.1f;
		m_Frame--;
	}
	else
	{
		m_BlendRate -= 2.0f;
		m_Frame++;
	}

	Enemy::HumanObject* enemyObject = nowscene->GetGameObject<Enemy::HumanObject>();

	// �G�I�u�W�F�N�g�̈ʒu���擾
	Vector3 enemyPosition = enemyObject->GetPosition();
	Vector3 toEnemy;
	toEnemy.x = enemyPosition.x - m_Position.x;
	toEnemy.y = enemyPosition.y - m_Position.y;
	toEnemy.z = enemyPosition.z - m_Position.z;

	// �v���C���[�I�u�W�F�N�g����G�I�u�W�F�N�g�ւ̕����x�N�g�����v�Z
	toEnemy.Normalize();
	// Y�����̉�]�p�x���v�Z
	float yaw = atan2f(toEnemy.x, toEnemy.z);
	// X�����̉�]�p�x���v�Z
	float pitch = -atan2f(toEnemy.y, sqrtf(toEnemy.x * toEnemy.x + toEnemy.z * toEnemy.z));
	// ��]��K�p
	m_Rotation = Vector3(pitch, yaw, m_Rotation.z);

	//�d��
	//m_Velocity.y -= 0.015f;

	//��R
	m_Velocity.y -= m_Velocity.y * 0.01f;

	//�ړ�
	m_Position += m_Velocity * moveSpeed;

	if (m_Position.x <= min.x) {
		m_Position.x = min.x;
	}
	if (m_Position.x >= max.x) {
		m_Position.x = max.x;
	}

	if (m_Position.z <= min.z) {
		m_Position.z = min.z;
	}
	if (m_Position.z >= max.z) {
		m_Position.z = max.z;
	}

	//�ڒn
	float groundHeight = fieldobj->GetFieldHeightBySqno(m_Position);

	// �ʒu���O�ȉ��Ȃ�n�ʈʒu�ɃZ�b�g����
	if (m_Position.y < groundHeight && m_Velocity.y < 0.0f)
	{
		m_Position.y = groundHeight;
		m_Velocity.y = 0.0f;
	}

	// ���݂̃V�[���̃v���C���[�̃I�u�W�F�N�g���擾
	Player::PlayerObject* player = nowscene->GetGameObject<Player::PlayerObject>();
	Evasive* playerEvasive = player->GetComponent<Evasive>();

	if (m_BlendRate > 1.0f)
	{
		m_BlendRate = 1.0f;
	}
	if (m_BlendRate < 0.0f)
	{
		m_BlendRate = 0.0f;
	}

	//�����蔻��
	playerHitSphere->SetCenter(m_Position);
	std::vector<Score*> score = nowscene->GetGameObjects<Score>();
	score.at(1)->SetCount(hp);
}

void Player::PlayerObject::PreDraw()
{
	//�}�e���A������
	// �f�o�C�X�R���e�L�X�g�擾
	ID3D11DeviceContext* devicecontext;
	devicecontext = Renderer::GetDeviceContext();

	// ���[���h�ϊ��s�񐶐�
	Matrix mtx;
	DX11MakeWorldMatrixRadian(
		mtx,
		m_Scale,							// �X�P�[��
		m_Rotation,							// �p��
		m_Position);						// �ʒu

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

void Player::PlayerObject::SetIsActive(bool _isActive)
{
	isActive = _isActive;
}

bool Player::PlayerObject::GetIsActive()
{
	return isActive;
}

void Player::PlayerObject::Damege(int damege)
{
	if (hp < damege)
	{
		hp = 0;
	}
	else
	{
		hp -= damege;
	}
}

int Player::PlayerObject::GetHp()
{
	return hp;
}

ActionModo Player::PlayerObject::GetActionModo()
{
	return actionModo;
}

void Player::PlayerObject::SetActionModo(ActionModo actionmodo)
{
	actionModo = actionmodo;
}