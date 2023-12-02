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
	//���݂̃V�[�����擾
	Scene* scene = Manager::GetScene();
	//���݂̃V�[���̃v���C���[�̃I�u�W�F�N�g���擾
	PlayerObject* player = scene->GetGameObject<PlayerObject>();

	//���݂̃V�[���̃J�����I�u�W�F�N�g���擾
	Camera* cameraobj = scene->GetGameObject<Camera>();

	Matrix viewmtx = cameraobj->GetViewMatrix();
	Vector3 zAxis = Vector3(viewmtx._13, 0.0f, viewmtx._33);
	Vector3 xAxis = Vector3(viewmtx._11, 0.0f, viewmtx._31);

	Vector3 playerPos = player->GetPosition();
	Vector3 playerLotation = player->GetRotation();

	//���E�ړ�
	if (Input::GetKeyPress('A'))
	{
		playerPos -= xAxis * m_SlideMoveSpeed;
	}
	if (Input::GetKeyPress('D'))
	{
		playerPos += xAxis * m_SlideMoveSpeed;
	}

	// �����L�[
	if (Input::GetKeyPress(LEFTARROW))
	{
		playerLotation.y -= 0.01f;
	}

	// �E���L�[
	if (Input::GetKeyPress(RIGHTARROW))
	{
		playerLotation.y += 0.01f;
	}

	// �O���x�N�g�����擾
	Vector3 forward = zAxis;

	if (Input::GetKeyPress('W'))
	{
		playerPos += forward * m_AroundMoveSpeed;
	}
	if (Input::GetKeyPress('S'))
	{
		playerPos -= forward * m_AroundMoveSpeed;
	}

	//�v���C���[�̍��W���X�V
	player->SetPosition(playerPos);
	player->SetRotation(playerLotation);
}