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

using namespace DirectX::SimpleMath;

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

	//’e”­ŽË
	m_BulletTime += 1.0f / 60.0f;

	if (m_BulletTime > 1.0f)
	{
		m_BulletTime -= 1.0f;

		Vector3 forward = GetForward();

		Scene* scene = Manager::GetScene();
		Bullet* bullet = scene->AddGameObject<Bullet>(2);
		bullet->SetPosition(m_Position + Vector3(0.0f, 1.5f, 0.0f));
		bullet->SetVelocity(forward * -0.5f);
	}

	if (m_BlendRate > 1.0f)
		m_BlendRate = 1.0f;
	if (m_BlendRate < 0.0f)
		m_BlendRate = 0.0f;
}

void Battery::PreDraw()
{
	m_Model->Update("Idle", m_Frame, "Run", m_Frame, m_BlendRate);
}