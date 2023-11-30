#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "modelRenderer.h"
#include "shader.h"
#include "Player.h"
#include "input.h"
#include "cylinder.h"
#include "box.h"
#include "audio.h"
#include "bullet.h"
#include "shadow.h"
#include "goal.h"
#include "collision.h"
#include "camera.h"
#include "animationModel.h"
#include "field.h"
#include "PlayerMove.h"
#include "PlayerFloating.h"
#include "PlayerEvasive.h"
#include "PlayerShot.h"

using namespace DirectX::SimpleMath;
using namespace Player;
using namespace Sound;


void PlayerObject::Init()
{
//	AddComponent<Shader>()->Load("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");  20230909-02
	AddComponent<Shader>()->Load("shader\\vertexLightingOneSkinVS.cso", "shader\\vertexLightingPS.cso"); //20230909-02

	m_Model = AddComponent<AnimationModel>();
	
	m_Model->Load("asset\\model\\Akai.fbx");									// animation ok
	m_Model->LoadAnimation("asset\\model\\Akai_Run.fbx", "Idle");
	m_Model->LoadAnimation("asset\\model\\Akai_Run.fbx", "Run");

//	m_Model->Load("asset\\model\\Akai2.fbx");									// animation ok
//	m_Model->LoadAnimation("asset\\model\\Akai_Walk.fbx", "Idle");
//	m_Model->LoadAnimation("asset\\model\\Akai_Walk.fbx", "Run");

//	m_Model->Load("data\\model\\Walking\\Walking2.fbx");						// animation ok
//	m_Model->LoadAnimation("data\\model\\Walking\\Walking2.fbx", "Idle");
//	m_Model->LoadAnimation("data\\model\\Walking\\Walking2.fbx", "Run");

//	m_Model->Load("data\\model\\Walking\\Walking.fbx");							// animation ok
//	m_Model->LoadAnimation("data\\model\\Walking\\Walking.fbx", "Idle");
//	m_Model->LoadAnimation("data\\model\\Walking\\Walking.fbx", "Run");

//	m_Model->Load("data\\model\\Walking\\Walking.dae");							// animation ok
//	m_Model->LoadAnimation("data\\model\\Walking\\Walking.dae", "Idle");		// animation ok
//	m_Model->LoadAnimation("data\\model\\Walking\\Walking.dae", "Run");			// animation ok
	

	AddComponent<Shadow>()->SetSize(1.5f);

	m_SE = AddComponent<Audio>();
	m_SE->Load("asset\\audio\\wan.wav");

	m_Scale = Vector3(0.01f, 0.01f, 0.01f);

	m_Delay = AddComponent<Timer::DelayCompnent>();
	m_Delay->SetLoop(true);
	m_Delay->SetdelayTime(0.2f);

	//プレイヤーのコンポーネント
	m_PlayerMove = AddComponent<Player::Move>();
	m_PlayerFloating = AddComponent<Player::Floating>();
	m_PlayerEvasive = AddComponent<Player::Evasive>();
	m_PlayerShot = AddComponent<Player::Shot>();


}

void PlayerObject::Update()
{
	Vector3 oldPosition = m_Position;

	Scene* nowscene = Manager::GetScene();
	Camera* cameraobj = nowscene->GetGameObject<Camera>();

	Matrix viewmtx = cameraobj->GetViewMatrix();
	Vector3 ZAxis = Vector3(viewmtx._13, 0.0f, viewmtx._33);
	Vector3 XAxis = Vector3(viewmtx._11, 0.0f, viewmtx._31);

	//前向きベクトルを取得
	Vector3 forward = ZAxis;

	//重力
	//m_Velocity.y -= 0.015f;

	//抵抗
	m_Velocity.y -= m_Velocity.y * 0.01f;

	//移動
	m_Position += m_Velocity;

	// フィールドオブジェクト取得
	Field* fieldobj = nowscene->GetGameObject<Field>();

	//　範囲チェック 
	Vector3 max = fieldobj->GetMax();
	Vector3 min = fieldobj->GetMin();

	if (m_Position.x <= min.x) {
		m_Position.x = min.x;
	}
	if (m_Position.x >= max.x) {
		m_Position.x = max.x;
	}

	if (m_Position.z <= min.z) {
		m_Position.z = min.z;
	}
	if (m_Position.z >= max.z) {
		m_Position.z = max.z;
	}

	//接地
	float groundHeight = fieldobj->GetFieldHeightBySqno(m_Position);

	// 位置が０以下なら地面位置にセットする
	if (m_Position.y < groundHeight && m_Velocity.y < 0.0f)
	{
		m_Position.y = groundHeight;
		m_Velocity.y = 0.0f;
	}
	

	if (Input::GetKeyPress('W'))
	{
		m_BlendRate += 0.1f;
		m_Frame++;

	}
	else if (Input::GetKeyPress('S'))
	{
		m_BlendRate += 0.1f;
		m_Frame--;
	}
	else
	{
		m_BlendRate -= 0.1f;
		m_Frame++;
	}

	if (m_BlendRate > 1.0f)
		m_BlendRate = 1.0f;
	if (m_BlendRate < 0.0f)
		m_BlendRate = 0.0f;

}


void PlayerObject::PreDraw()
{
	m_Model->Update("Idle", m_Frame, "Run", m_Frame, m_BlendRate);
}


