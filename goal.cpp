#include "modelRenderer.h"
#include "Goal.h"
#include "shader.h"
#include "shadow.h"

void Goal::Init()
{

	AddComponent<Shader>()->Load("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");
	AddComponent<ModelRenderer>()->Load("asset\\model\\goal.obj");


	AddComponent<Shadow>()->SetSize(1.5f);

}
