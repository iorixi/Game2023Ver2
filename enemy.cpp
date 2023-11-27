#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "scene.h"
#include "modelRenderer.h"
#include "enemy.h"
#include "shader.h"
//#include "shadow.h"


void Enemy::Init()
{

	AddComponent<Shader>()->Load("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");
	AddComponent<ModelRenderer>()->Load("asset\\model\\enemy.obj");

//	AddComponent<Shadow>()->SetSize(2.0f);

}


