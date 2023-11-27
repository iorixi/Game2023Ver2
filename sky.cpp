#include "main.h"
#include "manager.h"
#include "renderer.h"
//#include "input.h"
#include "scene.h"
#include "modelRenderer.h"
#include "sky.h"
#include "camera.h"
#include "shader.h"


using namespace DirectX::SimpleMath;

void Sky::Init()
{
	m_Scale = Vector3(100.0f, 100.0f, 100.0f);


	AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	AddComponent<ModelRenderer>()->Load("asset\\model\\sky.obj");

}


void Sky::Update()
{

	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();

	Vector3 cameraPosition = camera->GetPosition();
	m_Position = cameraPosition;

}


