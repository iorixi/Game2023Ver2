#include "CircleSkillShoot.h"
#include "scene.h"
#include "manager.h"
#include "modelRenderer.h"
#include "enemy.h"
#include "shadow.h"
#include "shader.h"
#include "score.h"
#include "BoundingSphere.h"
#include "HumanEnemy.h"
#include "player.h"
#include "CharaEnum.h"
#include "camera.h"
#include "ScheduledTask.h"
#include "CharaEnum.h"

using namespace DirectX::SimpleMath;

CircleSkillShoot::CircleSkillShoot()
{
}

CircleSkillShoot::~CircleSkillShoot()
{
}

void CircleSkillShoot::Load()
{
	ModelRenderer::Preload("asset\\model\\bullet.obj");
}

void CircleSkillShoot::Unload()
{
}

void CircleSkillShoot::Init()
{
	AddComponent<Shader>()->Load("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");
	AddComponent<ModelRenderer>()->Load("asset\\model\\bullet.obj");

	AddComponent<Shadow>()->SetSize(0.5f);

	//�q�I�u�W�F�N�g�ɓ����蔻���ǉ�
	m_Child = std::make_unique<BoundingSphereObj>(1, m_Position);

	//���Ԋ֌W������
	m_ShootTime = std::unique_ptr<Timer::ScheduledTask>();
}

void CircleSkillShoot::Update()
{
}