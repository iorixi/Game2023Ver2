#include "main.h"
#include "scene.h"
#include "manager.h"
#include "renderer.h"
#include "modelRenderer.h"
#include "bullet.h"
#include "enemy.h"
#include "shadow.h"
#include "shader.h"
#include "explosion.h"
#include "score.h"
#include "BoundingSphere.h"

using namespace DirectX::SimpleMath;
using namespace Enemy;

void Bullet::Load()
{
	ModelRenderer::Preload("asset\\model\\bullet.obj");
}

void Bullet::Unload()
{
}

void Bullet::Init()
{
	AddComponent<Shader>()->Load("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");
	AddComponent<ModelRenderer>()->Load("asset\\model\\bullet.obj");

	AddComponent<Shadow>()->SetSize(0.5f);

	//子オブジェクトに当たり判定を追加
	BoundingSphere* boundingSphere = new BoundingSphere(1, m_Position);
	m_Child = AddChild<BoundingSphere>();
	m_Child = boundingSphere;
}

void Bullet::Update()
{
	m_Position += m_Velocity;

	if (m_Position.Length() > 100.0f)
		SetDestroy();

	Scene* scene = Manager::GetScene();

	//当たり判定
	BoundingSphere* boundingSphere = GetBulletHitSphere();
	m_Child->SetCenter(m_Position);
}

//std::vector<EnemyObject*> enemyList = scene->GetGameObjects<EnemyObject>();
//
//	//敵への当たり判定
//	for (EnemyObject* enemy : enemyList)
//	{
//		Vector3 enemyPosition = enemy->GetPosition();
//
//		Vector3 direction = m_Position - enemyPosition;
//		float length = direction.Length();
//
//		if (length < 2.0f)
//		{
//			//scene->AddGameObject<Explosion>(2)->SetPosition(enemyPosition + Vector3(0.0f, 1.0f, 0.0f));
//
//			Score* score = scene->GetGameObject<Score>();
//			score->AddCount(10);
//
//			enemy->SetDestroy();
//			SetDestroy();
//			return;
//		}
//	}