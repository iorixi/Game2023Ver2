#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "modelRenderer.h"
#include "shader.h"
#include "player.h"
#include "input.h"
#include "cylinder.h"
#include "box.h"
#include "audio.h"
#include "bullet.h"
#include "shadow.h"

#include "goal.h"

#include "collision.h"

#include "camera.h"

#include "animationModel.h"

#include "field.h"

using namespace DirectX::SimpleMath;

void Player::Init()
{
//	AddComponent<Shader>()->Load("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");  20230909-02
	AddComponent<Shader>()->Load("shader\\vertexLightingOneSkinVS.cso", "shader\\vertexLightingPS.cso"); //20230909-02

	m_Model = AddComponent<AnimationModel>();
	
	m_Model->Load("asset\\model\\Akai.fbx");									// animation ok
	m_Model->LoadAnimation("asset\\model\\Akai_Run.fbx", "Idle");
	m_Model->LoadAnimation("asset\\model\\Akai_Run.fbx", "Run");

//	m_Model->Load("asset\\model\\Akai2.fbx");									// animation ok
//	m_Model->LoadAnimation("asset\\model\\Akai_Walk.fbx", "Idle");
//	m_Model->LoadAnimation("asset\\model\\Akai_Walk.fbx", "Run");

//	m_Model->Load("data\\model\\Walking\\Walking2.fbx");						// animation ok
//	m_Model->LoadAnimation("data\\model\\Walking\\Walking2.fbx", "Idle");
//	m_Model->LoadAnimation("data\\model\\Walking\\Walking2.fbx", "Run");

//	m_Model->Load("data\\model\\Walking\\Walking.fbx");							// animation ok
//	m_Model->LoadAnimation("data\\model\\Walking\\Walking.fbx", "Idle");
//	m_Model->LoadAnimation("data\\model\\Walking\\Walking.fbx", "Run");

//	m_Model->Load("data\\model\\Walking\\Walking.dae");							// animation ok
//	m_Model->LoadAnimation("data\\model\\Walking\\Walking.dae", "Idle");		// animation ok
//	m_Model->LoadAnimation("data\\model\\Walking\\Walking.dae", "Run");			// animation ok
	

	AddComponent<Shadow>()->SetSize(1.5f);

	m_SE = AddComponent<Audio>();
	m_SE->Load("asset\\audio\\wan.wav");

	m_Scale = Vector3(0.01f, 0.01f, 0.01f);
}

void Player::Update()
{
	Vector3 oldPosition = m_Position;

	Scene* nowscene = Manager::GetScene();
	Camera* cameraobj = nowscene->GetGameObject<Camera>();

	Matrix viewmtx = cameraobj->GetViewMatrix();
	Vector3 ZAxis = Vector3(viewmtx._13, 0.0f, viewmtx._33);
	Vector3 XAxis = Vector3(viewmtx._11, 0.0f, viewmtx._31);

	if (Input::GetKeyPress('A'))
	{
		m_Position -= XAxis * 0.05f;
	}
	if (Input::GetKeyPress('D'))
	{
		m_Position += XAxis * 0.05f;
	}

	// �����L�[
	if (Input::GetKeyPress(37))
	{
		m_Rotation.y -= 0.01f;
	}

	// �E���L�[
	if (Input::GetKeyPress(39))
	{
		m_Rotation.y += 0.01f;
	}

	// �O���x�N�g�����擾
//	Vector3 forward = GetForward();
	Vector3 forward = ZAxis;

	if (Input::GetKeyPress('W'))
	{
		m_Position += forward * 0.1f;
	}
	if (Input::GetKeyPress('S'))
	{
		m_Position -= forward * 0.1f;
	}

	//�W�����v
	if (Input::GetKeyTrigger('J'))
	{
		m_Velocity.y = 0.35f;
	}

	//�d��
	m_Velocity.y -= 0.015f;

	//��R
	m_Velocity.y -= m_Velocity.y * 0.01f;

	//�ړ�
	m_Position += m_Velocity;

	// �t�B�[���h�I�u�W�F�N�g�擾
	Field* fieldobj = nowscene->GetGameObject<Field>();

	//�@�͈̓`�F�b�N 
	Vector3 max = fieldobj->GetMax();
	Vector3 min = fieldobj->GetMin();

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

	//�e����
	if (Input::GetKeyTrigger('K'))
	{
		Scene* scene = Manager::GetScene();
		Bullet* bullet = scene->AddGameObject<Bullet>(2);
		bullet->SetPosition(m_Position + Vector3(0.0f, 1.0f, 0.0f));
		bullet->SetVelocity(forward * 0.5f);

		m_SE->Play();
	}

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
		m_BlendRate -= 0.1f;
		m_Frame++;
	}

	if (m_BlendRate > 1.0f)
		m_BlendRate = 1.0f;
	if (m_BlendRate < 0.0f)
		m_BlendRate = 0.0f;

}


void Player::PreDraw()
{
	m_Model->Update("Idle", m_Frame, "Run", m_Frame, m_BlendRate);
}


