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

	float m_CameraDistance = 5.0f;
	float m_PlayerTopThreshold = 2.0f;
	float m_CameraHeight = 1.8f;
	float m_CameraRightOffset = 0.8f;

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
};