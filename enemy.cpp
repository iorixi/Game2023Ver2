#include "enemy.h"
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "scene.h"
#include "modelRenderer.h"
#include "shader.h"
//#include "shadow.h"

using namespace Enemy;

void EnemyObject::Init()
{
	AddComponent<Shader>()->Load("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");
	AddComponent<ModelRenderer>()->Load("asset\\model\\enemy.obj");
	//	AddComponent<Shadow>()->SetSize(2.0f);
}

void Enemy::EnemyObject::Damege(int damege)
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

int Enemy::EnemyObject::GetHp()
{
	return hp;
}