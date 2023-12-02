#include "PlayerMove.h"
#include "input.h"
#include "player.h"
#include "scene.h"
#include "manager.h"
#include "camera.h"

using namespace DirectX::SimpleMath;
using namespace Player;

void Move::Init()
{
}

void Move::Update()
{
	//現在のシーンを取得
	Scene* scene = Manager::GetScene();
	//現在のシーンのプレイヤーのオブジェクトを取得
	PlayerObject* player = scene->GetGameObject<PlayerObject>();

	//現在のシーンのカメラオブジェクトを取得
	Camera* cameraobj = scene->GetGameObject<Camera>();

	Matrix viewmtx = cameraobj->GetViewMatrix();
	Vector3 zAxis = Vector3(viewmtx._13, 0.0f, viewmtx._33);
	Vector3 xAxis = Vector3(viewmtx._11, 0.0f, viewmtx._31);

	Vector3 playerPos = player->GetPosition();
	Vector3 playerLotation = player->GetRotation();

	//左右移動
	if (Input::GetKeyPress('A'))
	{
		playerPos -= xAxis * m_SlideMoveSpeed;
	}
	if (Input::GetKeyPress('D'))
	{
		playerPos += xAxis * m_SlideMoveSpeed;
	}

	// 左矢印キー
	if (Input::GetKeyPress(LEFTARROW))
	{
		playerLotation.y -= 0.01f;
	}

	// 右矢印キー
	if (Input::GetKeyPress(RIGHTARROW))
	{
		playerLotation.y += 0.01f;
	}

	// 前方ベクトルを取得
	Vector3 forward = zAxis;

	if (Input::GetKeyPress('W'))
	{
		playerPos += forward * m_AroundMoveSpeed;
	}
	if (Input::GetKeyPress('S'))
	{
		playerPos -= forward * m_AroundMoveSpeed;
	}

	//プレイヤーの座標を更新
	player->SetPosition(playerPos);
	player->SetRotation(playerLotation);
}