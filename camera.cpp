#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "Player.h"
#include "HumanEnemy.h"
#include "input.h"
#include <cmath> // �ǉ�����s

using namespace DirectX::SimpleMath;
using namespace Player;
using namespace Enemy;

void Camera::Init()
{
	m_Position = Vector3(0.0f, 10.0f, -50.0f);
	m_Target = Vector3(0.0f, 0.0f, 0.0f);
	m_FocusMidpoint = false;
	m_CameraMode = CameraMode::Normal;
}

void Camera::Uninit()
{
}

// ���ԓ_�ɏœ_�𓖂Ă邽�߂̃t���O��ݒ肷��֐���ǉ����܂��F
void Camera::SetFocusMidpoint(bool focusMidpoint)
{
	m_FocusMidpoint = focusMidpoint;
}

void Camera::Update()
{
	Scene* nowscene = Manager::GetScene();
	PlayerObject* playerObject = nowscene->GetGameObject<PlayerObject>();
	HumanObject* enemyObject = nowscene->GetGameObject<HumanObject>();

	Vector3 playerPosition = playerObject->GetPosition();
	Vector3 playerForward = playerObject->GetForward();
	Vector3 enemyPosition = enemyObject->GetPosition(); // �G�̈ʒu���擾

	// �v���C���[�ƓG�̊Ԃ̋������v�Z���܂�
	float distance = Vector3::Distance(playerPosition, enemyPosition);

	// ���������̋��������̏ꍇ�A���ԓ_�ɏœ_�𓖂Ă邽�߂̃t���O��ݒ肵�܂�
	if (round(distance) <= 10.0f) // �K�v�ɉ�����臒l�𒲐����Ă�������
	{
		// ���ԓ_�ɏœ_�𓖂Ă�t���O��ݒ肵�܂�
		m_CameraMode = CameraMode::CloseRange;
	}
	else
	{
		if (m_CameraMode == CameraMode::Transition)
		{
			m_CameraMode = CameraMode::Transition;
		}
		else
		{
			if (m_CameraMode == CameraMode::CloseRange)
			{
				m_CameraMode = CameraMode::Transition;
			}
			else
			{
				m_CameraMode = CameraMode::Normal;
			}
		}
	}

	Vector3 midpoint = (playerPosition + enemyPosition) * 0.5f;
	float lerpFactor = 0.05f; // �����\�ȌW��
	float cameraHeight;
	float playerVelocityY;
	float heightFactor;
	float pitch;
	float yaw;

	// ���݂̃��[�h�Ɋ�Â��ăJ�������X�V
	switch (m_CameraMode)
	{
	case CameraMode::Normal:

		// �ʏ�̏ꍇ�̃J�����̈ʒu�v�Z
		this->m_Position = playerPosition - playerForward * m_CameraDistance;
		// �J�����̈ʒu���Ē���
		playerPosition.y += m_CameraHeight;
		cameraHeight = 0.0f;
		playerVelocityY = playerObject->GetVelocity().y;

		if (playerVelocityY > 0.0f)
		{
			cameraHeight += playerVelocityY;
		}
		heightFactor = 1.0f - (playerPosition.y - enemyPosition.y) / m_CameraDistance;
		cameraHeight -= heightFactor;
		this->m_Position.y = playerPosition.y + cameraHeight;

		// �v���C���[�̕��𒍎��_�ɐݒ�
		this->m_Target = playerPosition;

		// �J�����̉�]�𒲐��i�v���C���[�̑O�����ɑ΂��ēK�؂Ȋp�x�j
		yaw = atan2f(playerForward.z, playerForward.x);
		this->m_Rotation.y = yaw;

		// �J�����̏㏸�p�x�𒲐��i�v���C���[�̑O�����Ə�����̂Ȃ��p�x�j
		pitch = atan2f(-playerForward.y, sqrtf(playerForward.x * playerForward.x + playerForward.z * playerForward.z));
		this->m_Rotation.x = pitch;

		break;

	case CameraMode::CloseRange:
		// �v���C���[�ƓG�̒��ԓ_���v�Z���܂�

		midpoint.y += m_CameraHeight;
		// ���X�ɒ��ԓ_�Ɍ����鏈��
		m_Target = Vector3::Lerp(m_Target, midpoint, lerpFactor);

		// �J�����̈ʒu���Ē���
		playerPosition.y += m_CameraHeight;
		cameraHeight = 0.0f;
		playerVelocityY = playerObject->GetVelocity().y;
		if (playerVelocityY > 0.0f)
		{
			cameraHeight += playerVelocityY;
		}
		heightFactor = 1.0f - (playerPosition.y - enemyPosition.y) / m_CameraDistance;
		cameraHeight -= heightFactor;
		this->m_Position.y = playerPosition.y + cameraHeight;

		// �J�����̉�]�𒲐��i�v���C���[�̑O�����ɑ΂��ēK�؂Ȋp�x�j
		yaw = atan2f(playerForward.z, playerForward.x);
		this->m_Rotation.y = yaw;

		// �J�����̏㏸�p�x�𒲐��i�v���C���[�̑O�����Ə�����̂Ȃ��p�x�j
		pitch = atan2f(-playerForward.y, sqrtf(playerForward.x * playerForward.x + playerForward.z * playerForward.z));
		this->m_Rotation.x = pitch;
		break;

		// �g�����W�V�������[�h�̏���
	case CameraMode::Transition:

		// �ʏ�̏ꍇ�̃J�����̈ʒu�v�Z
		this->m_Position = playerPosition - playerForward * m_CameraDistance;
		// �J�����̈ʒu���Ē���
		playerPosition.y += m_CameraHeight;
		cameraHeight = 0.0f;
		playerVelocityY = playerObject->GetVelocity().y;

		if (playerVelocityY > 0.0f)
		{
			cameraHeight += playerVelocityY;
		}
		heightFactor = 1.0f - (playerPosition.y - enemyPosition.y) / m_CameraDistance;
		cameraHeight -= heightFactor;
		this->m_Position.y = playerPosition.y + cameraHeight;

		// �v���C���[�̕��𒍎��_�ɐݒ�
		this->m_Target = playerPosition;

		// �J�����̉�]�𒲐��i�v���C���[�̑O�����ɑ΂��ēK�؂Ȋp�x�j
		yaw = atan2f(playerForward.z, playerForward.x);
		this->m_Rotation.y = yaw;

		// �J�����̏㏸�p�x�𒲐��i�v���C���[�̑O�����Ə�����̂Ȃ��p�x�j
		pitch = atan2f(-playerForward.y, sqrtf(playerForward.x * playerForward.x + playerForward.z * playerForward.z));
		this->m_Rotation.x = pitch;

		break;
	}
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