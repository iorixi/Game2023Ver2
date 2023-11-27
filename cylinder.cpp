#include "main.h"
#include "manager.h"
#include "renderer.h"
//#include "input.h"
//#include "scene.h"
#include "modelRenderer.h"
#include "cylinder.h"
//#include "shadow.h"
#include "shader.h"



void Cylinder::Init()
{

	AddComponent<Shader>()->Load("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");
	AddComponent<ModelRenderer>()->Load("asset\\model\\cylinder.obj");


//	AddComponent<Shadow>()->SetSize(5.0f);

}
