#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "modelRenderer.h"
#include "sky.h"
#include "camera.h"
#include "shader.h"
#include "AssimpModel.h"

using namespace DirectX::SimpleMath;

void AssimpModel::Init()
{
	m_Scale = Vector3(1.0f, 1.0f, 1.0f);


	AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	AddComponent<ModelRenderer>()->Load("asset\model\JACK式ピピ美_v1.0\\JACK式ピピ美1.0.pmx");

}


void AssimpModel::Update()
{

}


