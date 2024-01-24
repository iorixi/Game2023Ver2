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

	// �X�e�b�v���[�h���L�����ǂ���
	bool isStepMode = true;

	// ���������̋��������̏ꍇ�A�J�����̏�����ύX
	if (round(distance) <= 10.0f)
	{
		// �߂Â������̏���
		m_CameraMode = CameraMode::CloseRange;
	}
	else
	{
		//���������̋����ȏ�̏ꍇ�A�J�����̏�����ύX
		if (round(distance) >= 15.0f && m_CameraMode == CameraMode::CloseRange)
		{
			m_CameraMode = CameraMode::Normal;
		}
		else if (m_CameraMode == CameraMode::CloseRange)
		{
			m_CameraMode = CameraMode::CloseRange;
		}
		else
		{
			m_CameraMode = CameraMode::Normal;
		}
	}

	// Lerp �W���B�J�����̈ʒu�⒍���_�̕ύX���ɏ��X�Ɉړ����邽�߂Ɏg�p����܂��B
	float lerpFactor = 0.05f;

	// Transition ���[�h����ʏ탂�[�h�ɖ߂�ۂ� Lerp �W���B�ʏ탂�[�h�ɖ߂�ۂɂ����X�Ɉړ����邽�߂Ɏg�p����܂��B
	float returnLerpFactor = 0.05f;

	// �J�����̍����⑬�x�A�p�x�Ȃǂ̕ϐ��̐錾
	float cameraHeight;
	float playerVelocityY;
	float heightFactor;
	float pitch;
	float yaw;

	// �v���C���[�ƓG�̒��ԓ_���v�Z���܂�
	Vector3 midpoint = (playerPosition + enemyPosition) * 0.5f;

	// �J�����̖ڕW�ʒu��\���x�N�g��
	Vector3 targetPosition;

	//CloseRange�ϐ�

	//�i�G�ƃv���C���[�̒��������j�[�i�G�ƃv���C���[�̒��������j�����ꂼ����������̐�Βl
	float epecDistance = 10.0f;

	//�i�G�ƃv���C���[�̒��������j�[�i�G�ƃv���C���[�̒�������)��
	//���ꂼ����������̐�Βl�Ƃ̌덷�̋��e�͈�
	float epecDistanceDifference = 0.05f;

	//�G�ƃv���C���[�̒��������@- �G�ƃJ�����̒�������
	float currentEpecDistance;
	//�p�x
	float angle;

	// �v���C���[�̌��ɃJ�������ړ�������ڕW�ʒu
	Vector3 targetBehindPlayer;

	// �v���C���[����G�ւ̕����x�N�g��
	Vector3 directionToMove;

	// �v���C���[����G�ւ̕����x�N�g���𐳋K����������
	Vector3 directionToMoveNormalize;

	// �J�������ړ�������V�����ʒu
	Vector3 newCameraPosition;

	// �G����v���C���[�ւ̕����x�N�g��
	Vector3 toPlayer;

	// �G����J�����ւ̕����x�N�g��
	Vector3 toCamera;

	// �J�������ړ����������ڕW�ʒu
	Vector3 desiredCameraPosition;

	// �v���C���[�J�����̈ʒu
	Vector3 playerCamera;
	Vector3 enemyCameraTargetPos;

	// ���݂̃��[�h�Ɋ�Â��ăJ�������X�V
	switch (m_CameraMode)
	{
	case CameraMode::Normal:

		// �ʏ�̏ꍇ�̃J�����̈ʒu�v�Z
		playerPosition.y += m_CameraHeight;
		targetPosition = playerPosition - playerForward * (m_CameraDistance + (distance / m_CameraPosDistanceCorrection));

		// Lerp���g���ď��X�Ɉʒu�ƒ����_��ύX
		this->m_Position = Vector3::Lerp(this->m_Position, targetPosition, lerpFactor);
		enemyCameraTargetPos = enemyPosition;
		enemyCameraTargetPos.y += m_CameraTargetHeight * (distance / m_CameraTargetDistanceCorrection);

		this->m_Target = Vector3::Lerp(this->m_Target, enemyCameraTargetPos, lerpFactor);

		// �J�����̉�]�𒲐��i�v���C���[�̑O�����ɑ΂��ēK�؂Ȋp�x�j
		yaw = atan2f(playerForward.z, playerForward.x);
		this->m_Rotation.y = yaw;

		// �J�����̏㏸�p�x�𒲐��i�v���C���[�̑O�����Ə�����̂Ȃ��p�x�j
		pitch = atan2f(-playerForward.y, sqrtf(playerForward.x * playerForward.x + playerForward.z * playerForward.z));
		this->m_Rotation.x = pitch;

		break;

	case CameraMode::CloseRange:

		// �v���C���[�̌��ɃJ������ݒu�i�J�����ƃv���C���[�̋�����epec�̋����j�Ɍ�����Lerp
		targetBehindPlayer = playerPosition - playerForward * epecDistance;
		targetBehindPlayer.y += m_CameraHeightCloseRange;
		// �J�����ʒu�����X�ɕύX
		this->m_Position = Vector3::Lerp(this->m_Position, targetBehindPlayer, lerpFactor);

		// ���X�ɒ��ԓ_�Ɍ����鏈��
		midpoint.y += m_CameraHeightCloseRange;
		m_Target = Vector3::Lerp(m_Target, midpoint, lerpFactor);

		currentEpecDistance = Vector3::Distance(playerPosition, enemyPosition) - Vector3::Distance(enemyPosition, m_Position);

		// �J�����𓮂��������𖞂����ꍇ
		// epecDistance�̋�����epecDistanceDifference�̌덷�͈̔͂𒴂������ǂ���
		if (abs(currentEpecDistance - epecDistance) > epecDistanceDifference)
		{
			// �J�����𓮂��������𖞂����ꍇ
			// epec�̋�����epecDistance�̌덷��epecDistanceDifference�܂łɂȂ�悤�ɏ�ɃJ�����𓮂���
			directionToMove = playerPosition - enemyPosition;
			directionToMove.Normalize();// �x�N�g���𐳋K��

			//�V�����J����pos���`
			newCameraPosition = playerPosition + directionToMove * (currentEpecDistance + epecDistance);

			this->m_Position = Vector3::Lerp(this->m_Position, newCameraPosition, lerpFactor);
			m_Target = Vector3::Lerp(m_Target, midpoint, lerpFactor);  // �ēx���ԓ_�Ɍ����鏈�����s��
		}
		else
		{
			// �J�����𓮂��������𖞂����Ȃ��ꍇ�͒��ԓ_�Ɍ����鏈���̂ݍs��
			m_Target = Vector3::Lerp(m_Target, midpoint, lerpFactor);
		}

		// �G����v���C���[�ƓG����J�����̂Ȃ��p��70�x�ȏ�ɂȂ�悤�ɓ�����
		// �G����v���C���[�ւ̕����x�N�g��
		toPlayer = playerPosition - enemyPosition;
		// �G����J�����ւ̕����x�N�g��
		toCamera = m_Position - enemyPosition;

		//toPlayer �� toCamera �x�N�g���̓��ς��O�ς̐ςŊ��邱�Ƃɂ��A�x�N�g���Ԃ̊p�x���v�Z���Ă��܂��B
		//acos �֐��̓A�[�N�R�T�C�����v�Z���A���̌��ʂ��p�x�ƂȂ�܂��B
		angle = acos(toPlayer.Dot(toCamera) / (toPlayer.Length() * toCamera.Length()));

		if (angle > DirectX::XMConvertToRadians(70.0f))
		{
			// �J�����𓮂��������𖞂����ꍇ

			// �J�������v���C���[�̌��Ɉړ����邽�߂ɁA�v���C���[����G�ւ̕����x�N�g��(toPlayer)��
			// �w�肳�ꂽ�p�x������]�����A���̌��ʂ�ڕW�ʒu�Ƃ��Đݒ肵�܂��B
			desiredCameraPosition = enemyPosition + DirectX::SimpleMath::Vector3::Transform(toPlayer,
				DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(70.0f)));

			// �J�����̌��݂̈ʒu����ڕW�ʒu��Lerp���g�p���ď��X�Ɉړ������܂��B
			this->m_Position = Vector3::Lerp(this->m_Position, desiredCameraPosition, lerpFactor);

			// �J�����̒����_�����ԓ_��Lerp���g�p���ď��X�Ɍ����܂��B
			m_Target = Vector3::Lerp(m_Target, midpoint, lerpFactor);
			// �ēx���ԓ_�Ɍ����鏈�����s��
		}

		// �J�����̉�]�𒲐��i�v���C���[�̑O�����ɑ΂��ēK�؂Ȋp�x�j
		yaw = atan2f(playerForward.z, playerForward.x);
		this->m_Rotation.y = yaw;

		// �J�����̏㏸�p�x�𒲐��i�v���C���[�̑O�����Ə�����̂Ȃ��p�x�j
		pitch = atan2f(-playerForward.y, sqrtf(playerForward.x * playerForward.x + playerForward.z * playerForward.z));
		this->m_Rotation.x = pitch;

		break;
	}//switch
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