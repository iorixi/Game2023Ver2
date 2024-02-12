#include "enemy.h"
#include "manager.h"
#include "input.h"
#include "scene.h"
#include "modelRenderer.h"
#include "shader.h"

void Enemy::EnemyObject::Init()
{
	AddComponent<Shader>()->Load("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");
	AddComponent<ModelRenderer>()->Load("asset\\model\\enemy.obj");
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