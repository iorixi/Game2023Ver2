#include "manager.h"
#include "renderer.h"
#include "shader.h"
#include "Player.h"
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
#include "PlayerMove.h"
#include "PlayerFloating.h"
#include "PlayerEvasive.h"
#include "PlayerShot.h"
#include "HumanEnemy.h"
#include "BoundingSphere.h"
#include "ActionModo.h"
#include "score.h"
#include <vector>

using namespace DirectX::SimpleMath;

//�v���C���[�̈ړ����x
const float moveSpeed = 1.5f;

void Player::PlayerObject::Init()
{
	//	AddComponent<Shader>()->Load("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");  20230909-02
	AddComponent<Shader>()->Load("shader\\vertexLightingOneSkinVS.cso", "shader\\vertexLightingPS.cso"); //20230909-02

	m_Model = AddComponent<AnimationModel>();

	m_Model->Load("asset\\model\\KachujinGRosales.fbx");									// animation ok
	m_Model->LoadAnimation("asset\\model\\Stand.fbx", "Idle");
	m_Model->LoadAnimation("asset\\model\\Evasise.fbx", "Evasise");

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

	m_SE = AddComponent<Sound::Audio>();
	m_SE->Load("asset\\audio\\damage.wav");

	m_Scale = Vector3(0.015f, 0.015f, 0.015f);

	m_Delay = AddComponent<Timer::DelayCompnent>();
	m_Delay->SetLoop(true);
	m_Delay->SetdelayTime(0.2f);

	//�v���C���[�̃R���|�[�l���g
	m_PlayerMove = AddComponent<Player::Move>();
	m_PlayerFloating = AddComponent<Player::Floating>();
	m_PlayerEvasive = AddComponent<Player::Evasive>();
	m_PlayerShot = AddComponent<Player::Shot>();
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
	// ���݂̃V�[�����擾
	Scene* currentScene = Manager::GetScene();
	// ���݂̃V�[���̃v���C���[�̃I�u�W�F�N�g���擾
	Player::PlayerObject* player = currentScene->GetGameObject<Player::PlayerObject>();
	Evasive* playerEvasive = player->GetComponent<Evasive>();

	if (playerEvasive->GetAnimationFlg())
	{
		m_Model->Update("Idle", m_Frame, "Idle", m_Frame, m_BlendRate);
	}
	else
	{
		m_Model->Update("Idle", m_Frame, "Idle", m_Frame, m_BlendRate);
	}
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