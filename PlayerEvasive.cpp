#include "PlayerEvasive.h"
#include "input.h"
#include "player.h"
#include "scene.h"
#include "manager.h"

using namespace DirectX::SimpleMath;
using namespace Player;

void Evasive::Init()
{
}

void Evasive::Update()
{
	//現在のシーンを取得
	Scene* scene = Manager::GetScene();
	//現在のシーンのプレイヤーのオブジェクトを取得
	PlayerObject* player = scene->GetGameObject<PlayerObject>();

	//方向を取得
	Vector3 playerVelocity = player->GetVelocity();

	//回転
	Vector3 playerRotation = player->GetRotation();
	//playerRotation.z -= 0.01f;
	player->SetRotation(playerRotation);

	player->SetVelocity(playerVelocity);
}