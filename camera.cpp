#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "Player.h"
#include "HumanEnemy.h"

using namespace DirectX::SimpleMath;
using namespace Player;
using namespace Enemy;

void Camera::Init()
{
	m_Position = Vector3(0.0f, 10.0f, -50.0f);
	m_Target = Vector3(0.0f, 0.0f, 0.0f);
}

void Camera::Uninit()
{
}

void Camera::Update()
{
	Scene* nowscene = Manager::GetScene();
	PlayerObject* playerObject = nowscene->GetGameObject<PlayerObject>();
	HumanObject* enemyObject = nowscene->GetGameObject<HumanObject>();

	Vector3 playerPosition = playerObject->GetPosition();
	Vector3 playerForward = playerObject->GetForward();
	Vector3 enemyPosition = enemyObject->GetPosition(); // �G�̈ʒu���擾

	// �J�����̈ʒu��ݒ�i�v���C���[�̌��ɔz�u�j
	this->m_Position = playerPosition - playerForward * m_CameraDistance;

	//�J�����̈ʒu���Ē���
	playerPosition.y += m_CameraHeight;

	// �J�����̍����𒲐�
	float cameraHeight = 0.0f;

	// �v���C���[���㏸���̏ꍇ�A�㏸�ʂ��l�����ăJ�����̍����𒲐�
	float playerVelocityY = playerObject->GetVelocity().y;
	if (playerVelocityY > 0.0f)
	{
		cameraHeight += playerVelocityY;
	}

	float heightFactor = 1.0f - (playerPosition.y - enemyPosition.y) / m_CameraDistance;
	cameraHeight -= heightFactor;

	// �J�����̍����𒲐�
	this->m_Position.y = playerPosition.y + cameraHeight;

	// �v���C���[�̕��𒍎��_�ɐݒ�
	this->m_Target = playerPosition;

	// �J�����̉�]�𒲐��i�v���C���[�̑O�����ɑ΂��ēK�؂Ȋp�x�j
	float yaw = atan2f(playerForward.z, playerForward.x);
	this->m_Rotation.y = yaw;

	// �J�����̏㏸�p�x�𒲐��i�v���C���[�̑O�����Ə�����̂Ȃ��p�x�j
	float pitch = atan2f(-playerForward.y, sqrtf(playerForward.x * playerForward.x + playerForward.z * playerForward.z));
	this->m_Rotation.x = pitch;
}

void Camera::Draw()
{
	// �r���[�ϊ����쐬
	Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
	Vector3 pos = m_Position;
	Scene* nowscene = Manager::GetScene();
	PlayerObject* playerObject = nowscene->GetGameObject<PlayerObject>();
	Vector3 cameraPosition = m_Position + m_CameraRightOffset * playerObject->GetRight();

	m_ViewMatrix = DirectX::XMMatrixLookAtLH(cameraPosition, m_Target, up);									// ����n�ɂ����@20230511 by suzuki.tomoki

	// DIRECTXTK�̃��\�b�h�͉E��n�@20230511 by suzuki.tomoki
	// �E��n�ɂ���ƂR�p�`���_�������v���ɂȂ�̂ŕ`�悳��Ȃ��Ȃ�̂Œ���
	// ���̃R�[�h�͊m�F�e�X�g�̂��߂Ɏc��
	//	m_ViewMatrix = m_ViewMatrix.CreateLookAt(m_Position, m_Target, up);

	Renderer::SetViewMatrix(&m_ViewMatrix);

	//�v���W�F�N�V�����s��̐���
	constexpr float fieldOfView = DirectX::XMConvertToRadians(45.0f);    // ����p

	float aspectRatio = static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT);	// �A�X�y�N�g��
	float nearPlane = 1.0f;       // �j�A�N���b�v
	float farPlane = 1000.0f;      // �t�@�[�N���b�v

	//�v���W�F�N�V�����s��̐���
	Matrix projectionMatrix;
	projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, aspectRatio, nearPlane, farPlane);	// ����n�ɂ����@20230511 by suzuki.tomoki

	// DIRECTXTK�̃��\�b�h�͉E��n�@20230511 by suzuki.tomoki
	// �E��n�ɂ���ƂR�p�`���_�������v���ɂȂ�̂ŕ`�悳��Ȃ��Ȃ�̂Œ���
	// ���̃R�[�h�͊m�F�e�X�g�̂��߂Ɏc��
//	projectionMatrix = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(fieldOfView, aspectRatio, nearPlane, farPlane);

	Renderer::SetProjectionMatrix(&projectionMatrix);
}