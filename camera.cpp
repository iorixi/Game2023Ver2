#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "Player.h"
#include "HumanEnemy.h"
#include "input.h"
#include <cmath> // 追加する行

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

// 中間点に焦点を当てるためのフラグを設定する関数を追加します：
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
	Vector3 enemyPosition = enemyObject->GetPosition(); // 敵の位置を取得

	// プレイヤーと敵の間の距離を計算します
	float distance = Vector3::Distance(playerPosition, enemyPosition);

	// 距離が一定の距離未満の場合、中間点に焦点を当てるためのフラグを設定します
	if (round(distance) <= 10.0f) // 必要に応じて閾値を調整してください
	{
		// 中間点に焦点を当てるフラグを設定します
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
	float lerpFactor = 0.05f; // 調整可能な係数
	float cameraHeight;
	float playerVelocityY;
	float heightFactor;
	float pitch;
	float yaw;

	// 現在のモードに基づいてカメラを更新
	switch (m_CameraMode)
	{
	case CameraMode::Normal:

		// 通常の場合のカメラの位置計算
		this->m_Position = playerPosition - playerForward * m_CameraDistance;
		// カメラの位置を再調整
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

		// プレイヤーの方を注視点に設定
		this->m_Target = playerPosition;

		// カメラの回転を調整（プレイヤーの前方向に対して適切な角度）
		yaw = atan2f(playerForward.z, playerForward.x);
		this->m_Rotation.y = yaw;

		// カメラの上昇角度を調整（プレイヤーの前方向と上方向のなす角度）
		pitch = atan2f(-playerForward.y, sqrtf(playerForward.x * playerForward.x + playerForward.z * playerForward.z));
		this->m_Rotation.x = pitch;

		break;

	case CameraMode::CloseRange:
		// プレイヤーと敵の中間点を計算します

		midpoint.y += m_CameraHeight;
		// 徐々に中間点に向ける処理
		m_Target = Vector3::Lerp(m_Target, midpoint, lerpFactor);

		// カメラの位置を再調整
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

		// カメラの回転を調整（プレイヤーの前方向に対して適切な角度）
		yaw = atan2f(playerForward.z, playerForward.x);
		this->m_Rotation.y = yaw;

		// カメラの上昇角度を調整（プレイヤーの前方向と上方向のなす角度）
		pitch = atan2f(-playerForward.y, sqrtf(playerForward.x * playerForward.x + playerForward.z * playerForward.z));
		this->m_Rotation.x = pitch;
		break;

		// トランジションモードの処理
	case CameraMode::Transition:

		// 通常の場合のカメラの位置計算
		this->m_Position = playerPosition - playerForward * m_CameraDistance;
		// カメラの位置を再調整
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

		// プレイヤーの方を注視点に設定
		this->m_Target = playerPosition;

		// カメラの回転を調整（プレイヤーの前方向に対して適切な角度）
		yaw = atan2f(playerForward.z, playerForward.x);
		this->m_Rotation.y = yaw;

		// カメラの上昇角度を調整（プレイヤーの前方向と上方向のなす角度）
		pitch = atan2f(-playerForward.y, sqrtf(playerForward.x * playerForward.x + playerForward.z * playerForward.z));
		this->m_Rotation.x = pitch;

		break;
	}
}

void Camera::Draw()
{
	// ビュー変換後列作成
	Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
	Vector3 pos = m_Position;
	Scene* nowscene = Manager::GetScene();
	PlayerObject* playerObject = nowscene->GetGameObject<PlayerObject>();
	Vector3 cameraPosition = m_Position + m_CameraRightOffset * playerObject->GetRight();

	m_ViewMatrix = DirectX::XMMatrixLookAtLH(cameraPosition, m_Target, up);									// 左手系にした　20230511 by suzuki.tomoki

	// DIRECTXTKのメソッドは右手系　20230511 by suzuki.tomoki
	// 右手系にすると３角形頂点が反時計回りになるので描画されなくなるので注意
	// このコードは確認テストのために残す
	//	m_ViewMatrix = m_ViewMatrix.CreateLookAt(m_Position, m_Target, up);

	Renderer::SetViewMatrix(&m_ViewMatrix);

	//プロジェクション行列の生成
	constexpr float fieldOfView = DirectX::XMConvertToRadians(45.0f);    // 視野角

	float aspectRatio = static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT);	// アスペクト比
	float nearPlane = 1.0f;       // ニアクリップ
	float farPlane = 1000.0f;      // ファークリップ

	//プロジェクション行列の生成
	Matrix projectionMatrix;
	projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, aspectRatio, nearPlane, farPlane);	// 左手系にした　20230511 by suzuki.tomoki

	// DIRECTXTKのメソッドは右手系　20230511 by suzuki.tomoki
	// 右手系にすると３角形頂点が反時計回りになるので描画されなくなるので注意
	// このコードは確認テストのために残す
//	projectionMatrix = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(fieldOfView, aspectRatio, nearPlane, farPlane);

	Renderer::SetProjectionMatrix(&projectionMatrix);
}