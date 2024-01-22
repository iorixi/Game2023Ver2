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
	//���݂̃V�[�����擾
	Scene* scene = Manager::GetScene();
	//���݂̃V�[���̃v���C���[�̃I�u�W�F�N�g���擾
	PlayerObject* player = scene->GetGameObject<PlayerObject>();

	//�������擾
	Vector3 playerVelocity = player->GetVelocity();

	//��]
	Vector3 playerRotation = player->GetRotation();
	//playerRotation.z -= 0.01f;
	player->SetRotation(playerRotation);

	player->SetVelocity(playerVelocity);
}