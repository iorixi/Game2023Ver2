#include "HomingBullet.h"
#include "main.h"
#include "scene.h"
#include "manager.h"
#include "renderer.h"
#include "modelRenderer.h"
#include "enemy.h"
#include "shadow.h"
#include "shader.h"
#include "explosion.h"
#include "score.h"
#include "BoundingSphere.h"

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

	//子オブジェクトに当たり判定を追加
	BoundingSphere* boundingSphere = new BoundingSphere(1, m_Position);
	m_Child = AddChild<BoundingSphere>();
	m_Child = boundingSphere;
}

void HomingBullet::Update()
{
	m_Position += m_Velocity;

	if (m_Position.Length() > 100.0f)
		SetDestroy();

	Scene* scene = Manager::GetScene();

	//当たり判定
	BoundingSphere* boundingSphere = GetBulletHitSphere();
	m_Child->SetCenter(m_Position);
}