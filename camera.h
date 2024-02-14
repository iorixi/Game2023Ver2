#pragma once

#include	<SimpleMath.h>
#include	"gameObject.h"

enum class CameraMode
{
	Normal,
	CloseRange,
	Transition
};

class Camera : public GameObject {
private:

	DirectX::SimpleMath::Vector3	m_Target{};
	DirectX::SimpleMath::Matrix		m_ViewMatrix{};

	float m_CameraDistance = 8.0f;						//�J�����̋���
	float m_PlayerTopThreshold = 2.0f;					//�v���C���[�L�����N�^�[�̓��̈ʒu
	float m_CameraHeight = 4.0f;						//�ʏ탂�[�h�̎��̃J�����̍���
	float m_CameraHeightCloseRange = 2.2f;				//�ߋ������[�h�̎��̃J�����̍���
	float m_CameraTargetHeight = -6.8f;					//�J�����̒����_�̍���
	float m_CameraRightOffset = 1.2f;					//�J�������E�Ɋ񂹂�
	float m_CameraRotatePitchPower = 0.5f;				//�J������X��]�̕␳�{��
	float m_CameraPosDistanceCorrection = 100.0f;		//�J�����̈ʒu�̋����␳
	float m_CameraTargetDistanceCorrection = 100.0f;	//�J�����̒����_�̋����␳

	// Lerp �W���B�J�����̈ʒu�⒍���_�̕ύX���ɏ��X�Ɉړ����邽�߂Ɏg�p����܂��B
	float lerpFactor = 0.07f;
	//�����ɍ��킹�ăJ�����̑��x��␳
	float distanceFactor = 0.02f;

	//�����_
	bool m_FocusMidpoint;

	CameraMode m_CameraMode;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetFocusMidpoint(bool focusMidpoint);

	DirectX::SimpleMath::Matrix GetViewMatrix()
	{
		return m_ViewMatrix;
	}

private:
	//imgui�̒l������Ă���
	void GetImguiCameraUpdate();
};