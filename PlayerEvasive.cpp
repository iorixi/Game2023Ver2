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

	Vector3 playerVelocity = player->GetVelocity();

	//playerRotation.z -= 0.01;

	player->SetVelocity(playerVelocity);
}