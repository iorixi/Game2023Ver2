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

	float m_CameraDistance = 8.0f;						//カメラの距離
	float m_PlayerTopThreshold = 2.0f;					//プレイヤーキャラクターの頭の位置
	float m_CameraHeight = 4.0f;						//通常モードの時のカメラの高さ
	float m_CameraHeightCloseRange = 2.2f;				//近距離モードの時のカメラの高さ
	float m_CameraTargetHeight = -6.8f;					//カメラの中視点の高さ
	float m_CameraRightOffset = 1.2f;					//カメラを右に寄せる
	float m_CameraRotatePitchPower = 0.5f;				//カメラのX回転の補正倍率
	float m_CameraPosDistanceCorrection = 100.0f;		//カメラの位置の距離補正
	float m_CameraTargetDistanceCorrection = 100.0f;	//カメラの中視点の距離補正

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