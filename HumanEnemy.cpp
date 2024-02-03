#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "scene.h"
#include "modelRenderer.h"
#include "HumanEnemy.h"
#include "shader.h"
//#include "shadow.h"
#include "bullet.h"
#include "animationModel.h"
#include "player.h"
#include "HomingBullet.h"
#include "CharaEnum.h"
#include "HumanEnemyBullet.h"
#include "BoundingSphere.h"
#include "HumanEnemyMove.h"

using namespace DirectX::SimpleMath;
using namespace Player;
using namespace Enemy;

void HumanObject::Init()
{
	//AddComponent<Shader>()->Load("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");
	//AddComponent<ModelRenderer>()->Load("asset\\model\\battery1.obj");
	//
	////	AddComponent<Shadow>()->SetSize(3.0f);
	//
	//enemyHitSphere = AddChild<GameObject>();
	//enemyHitSphere->AddComponent<Shader>()->Load("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");
	//enemyHitSphere->AddComponent<ModelRenderer>()->Load("asset\\model\\battery2.obj");

	//	AddComponent<Shader>()->Load("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");  20230909-02
	AddComponent<Shader>()->Load("shader\\vertexLightingOneSkinVS.cso", "shader\\vertexLightingPS.cso"); //20230909-02

	m_Model = AddComponent<AnimationModel>();

	m_Model->Load("asset\\model\\Akai.fbx");									// animation ok
	m_Model->LoadAnimation("asset\\model\\Stand.fbx", "Idle");
	m_Model->LoadAnimation("asset\\model\\Stand.fbx", "Idle");

	m_Position.y = 10;

	m_Scale = Vector3(0.015f, 0.015f, 0.015f);
	//子オブジェクトに当たり判定を追加
	enemyHitSphere = std::make_shared<BoundingSphereObj>(0.1f, m_Position);

	//m_EnemyMove = AddComponent<Enemy::Move>();
	m_EnemyShot = AddComponent<Enemy::Shot>();
}

void HumanObject::Update()
{
	Scene* nowscene = Manager::GetScene();

	m_MoveTime += 1.0f / 60.0f;

	//m_Position.z += cosf(m_MoveTime * 1.0f) * 0.1f;
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

	// プレイヤーオブジェクトの位置を取得
	PlayerObject* playerObject = nowscene->GetGameObject<PlayerObject>();
	Vector3 playerPos = playerObject->GetPosition();
	Vector3 toPlayer;
	toPlayer.x = playerPos.x - m_Position.x;
	toPlayer.y = playerPos.y - m_Position.y;
	toPlayer.z = playerPos.z - m_Position.z;

	// プレイヤーオブジェクトから敵オブジェクトへの方向ベクトルを計算
	toPlayer.Normalize();

	// Y軸回りの回転角度を計算
	float yaw = atan2f(toPlayer.x, toPlayer.z);
	// X軸回りの回転角度を計算
	float pitch = atan2f(toPlayer.y, sqrtf(toPlayer.x * toPlayer.x + toPlayer.z * toPlayer.z));

	// Z軸回りの回転角度（このサンプルでは固定で0.0fとしています）
	float roll = 0.0f;

	// 回転を適用
	m_Rotation = Vector3(pitch, yaw, roll);

	//当たり判定
	enemyHitSphere->SetCenter(m_Position);
}

void HumanObject::PreDraw()
{
	m_Model->Update("Idle", m_Frame, "Idle", m_Frame, m_BlendRate);
}

void Enemy::HumanObject::SetIsActive(bool _isActive)
{
	isActive = _isActive;
}

bool Enemy::HumanObject::GetIsActive()
{
	return isActive;
}

ActionModo Enemy::HumanObject::GetActionModo()
{
	return actionModo;
}

void Enemy::HumanObject::SetActionModo(ActionModo actionmodo)
{
	actionModo = actionmodo;
}