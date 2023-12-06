#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "scene.h"
#include "modelRenderer.h"
#include "battery.h"
#include "shader.h"
//#include "shadow.h"
#include "bullet.h"
#include "animationModel.h"
#include "player.h"

using namespace DirectX::SimpleMath;
using namespace Player;

void Battery::Init()
{
	//AddComponent<Shader>()->Load("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");
	//AddComponent<ModelRenderer>()->Load("asset\\model\\battery1.obj");
	//
	////	AddComponent<Shadow>()->SetSize(3.0f);
	//
	//m_Child = AddChild<GameObject>();
	//m_Child->AddComponent<Shader>()->Load("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");
	//m_Child->AddComponent<ModelRenderer>()->Load("asset\\model\\battery2.obj");

	//	AddComponent<Shader>()->Load("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");  20230909-02
	AddComponent<Shader>()->Load("shader\\vertexLightingOneSkinVS.cso", "shader\\vertexLightingPS.cso"); //20230909-02

	m_Model = AddComponent<AnimationModel>();

	m_Model->Load("asset\\model\\Akai.fbx");									// animation ok
	m_Model->LoadAnimation("asset\\model\\Akai_Run.fbx", "Idle");
	m_Model->LoadAnimation("asset\\model\\Akai_Run.fbx", "Run");

	m_Scale = Vector3(0.01f, 0.01f, 0.01f);

	m_Position.y = 10;
}

void Battery::Update()
{
	Scene* nowscene = Manager::GetScene();

	m_MoveTime += 1.0f / 60.0f;

	m_Position.z += cosf(m_MoveTime * 1.0f) * 0.1f;
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

	//指定時間
	m_BulletTime += 1.0f / 60.0f;

	//弾発射
	if (m_BulletTime > 1.0f)
	{
		m_BulletTime -= 1.0f;

		Vector3 forward = GetForward();

		Scene* nowscene = Manager::GetScene();
		Bullet* bullet = nowscene->AddGameObject<Bullet>(2);
		bullet->SetPosition(m_Position + Vector3(0.0f, 1.5f, 0.0f));
		bullet->SetVelocity(forward * -0.5f);
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
}

void Battery::PreDraw()
{
	m_Model->Update("Idle", m_Frame, "Run", m_Frame, m_BlendRate);
}