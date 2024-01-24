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

	// ステップモードが有効かどうか
	bool isStepMode = true;

	// 距離が一定の距離未満の場合、カメラの処理を変更
	if (round(distance) <= 10.0f)
	{
		// 近づいた時の処理
		m_CameraMode = CameraMode::CloseRange;
	}
	else
	{
		//距離が一定の距離以上の場合、カメラの処理を変更
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

	// Lerp 係数。カメラの位置や注視点の変更時に徐々に移動するために使用されます。
	float lerpFactor = 0.05f;

	// Transition モードから通常モードに戻る際の Lerp 係数。通常モードに戻る際にも徐々に移動するために使用されます。
	float returnLerpFactor = 0.05f;

	// カメラの高さや速度、角度などの変数の宣言
	float cameraHeight;
	float playerVelocityY;
	float heightFactor;
	float pitch;
	float yaw;

	// プレイヤーと敵の中間点を計算します
	Vector3 midpoint = (playerPosition + enemyPosition) * 0.5f;

	// カメラの目標位置を表すベクトル
	Vector3 targetPosition;

	//CloseRange変数

	//（敵とプレイヤーの直線距離）ー（敵とプレイヤーの直線距離）をそれぞれ引いた時の絶対値
	float epecDistance = 10.0f;

	//（敵とプレイヤーの直線距離）ー（敵とプレイヤーの直線距離)を
	//それぞれ引いた時の絶対値との誤差の許容範囲
	float epecDistanceDifference = 0.05f;

	//敵とプレイヤーの直線距離　- 敵とカメラの直線距離
	float currentEpecDistance;
	//角度
	float angle;

	// プレイヤーの後ろにカメラを移動させる目標位置
	Vector3 targetBehindPlayer;

	// プレイヤーから敵への方向ベクトル
	Vector3 directionToMove;

	// プレイヤーから敵への方向ベクトルを正規化したもの
	Vector3 directionToMoveNormalize;

	// カメラを移動させる新しい位置
	Vector3 newCameraPosition;

	// 敵からプレイヤーへの方向ベクトル
	Vector3 toPlayer;

	// 敵からカメラへの方向ベクトル
	Vector3 toCamera;

	// カメラを移動させたい目標位置
	Vector3 desiredCameraPosition;

	// プレイヤーカメラの位置
	Vector3 playerCamera;
	Vector3 enemyCameraTargetPos;

	// 現在のモードに基づいてカメラを更新
	switch (m_CameraMode)
	{
	case CameraMode::Normal:

		// 通常の場合のカメラの位置計算
		playerPosition.y += m_CameraHeight;
		targetPosition = playerPosition - playerForward * (m_CameraDistance + (distance / m_CameraPosDistanceCorrection));

		// Lerpを使って徐々に位置と注視点を変更
		this->m_Position = Vector3::Lerp(this->m_Position, targetPosition, lerpFactor);
		enemyCameraTargetPos = enemyPosition;
		enemyCameraTargetPos.y += m_CameraTargetHeight * (distance / m_CameraTargetDistanceCorrection);

		this->m_Target = Vector3::Lerp(this->m_Target, enemyCameraTargetPos, lerpFactor);

		// カメラの回転を調整（プレイヤーの前方向に対して適切な角度）
		yaw = atan2f(playerForward.z, playerForward.x);
		this->m_Rotation.y = yaw;

		// カメラの上昇角度を調整（プレイヤーの前方向と上方向のなす角度）
		pitch = atan2f(-playerForward.y, sqrtf(playerForward.x * playerForward.x + playerForward.z * playerForward.z));
		this->m_Rotation.x = pitch;

		break;

	case CameraMode::CloseRange:

		// プレイヤーの後ろにカメラを設置（カメラとプレイヤーの距離はepecの距離）に向けてLerp
		targetBehindPlayer = playerPosition - playerForward * epecDistance;
		targetBehindPlayer.y += m_CameraHeightCloseRange;
		// カメラ位置を徐々に変更
		this->m_Position = Vector3::Lerp(this->m_Position, targetBehindPlayer, lerpFactor);

		// 徐々に中間点に向ける処理
		midpoint.y += m_CameraHeightCloseRange;
		m_Target = Vector3::Lerp(m_Target, midpoint, lerpFactor);

		currentEpecDistance = Vector3::Distance(playerPosition, enemyPosition) - Vector3::Distance(enemyPosition, m_Position);

		// カメラを動かす条件を満たす場合
		// epecDistanceの距離がepecDistanceDifferenceの誤差の範囲を超えたかどうか
		if (abs(currentEpecDistance - epecDistance) > epecDistanceDifference)
		{
			// カメラを動かす条件を満たす場合
			// epecの距離がepecDistanceの誤差がepecDistanceDifferenceまでになるように常にカメラを動かす
			directionToMove = playerPosition - enemyPosition;
			directionToMove.Normalize();// ベクトルを正規化

			//新しいカメラposを定義
			newCameraPosition = playerPosition + directionToMove * (currentEpecDistance + epecDistance);

			this->m_Position = Vector3::Lerp(this->m_Position, newCameraPosition, lerpFactor);
			m_Target = Vector3::Lerp(m_Target, midpoint, lerpFactor);  // 再度中間点に向ける処理を行う
		}
		else
		{
			// カメラを動かす条件を満たさない場合は中間点に向ける処理のみ行う
			m_Target = Vector3::Lerp(m_Target, midpoint, lerpFactor);
		}

		// 敵からプレイヤーと敵からカメラのなす角が70度以上になるように動かす
		// 敵からプレイヤーへの方向ベクトル
		toPlayer = playerPosition - enemyPosition;
		// 敵からカメラへの方向ベクトル
		toCamera = m_Position - enemyPosition;

		//toPlayer と toCamera ベクトルの内積を外積の積で割ることにより、ベクトル間の角度を計算しています。
		//acos 関数はアークコサインを計算し、その結果が角度となります。
		angle = acos(toPlayer.Dot(toCamera) / (toPlayer.Length() * toCamera.Length()));

		if (angle > DirectX::XMConvertToRadians(70.0f))
		{
			// カメラを動かす条件を満たす場合

			// カメラをプレイヤーの後ろに移動するために、プレイヤーから敵への方向ベクトル(toPlayer)を
			// 指定された角度だけ回転させ、その結果を目標位置として設定します。
			desiredCameraPosition = enemyPosition + DirectX::SimpleMath::Vector3::Transform(toPlayer,
				DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(70.0f)));

			// カメラの現在の位置から目標位置にLerpを使用して徐々に移動させます。
			this->m_Position = Vector3::Lerp(this->m_Position, desiredCameraPosition, lerpFactor);

			// カメラの注視点も中間点にLerpを使用して徐々に向けます。
			m_Target = Vector3::Lerp(m_Target, midpoint, lerpFactor);
			// 再度中間点に向ける処理を行う
		}

		// カメラの回転を調整（プレイヤーの前方向に対して適切な角度）
		yaw = atan2f(playerForward.z, playerForward.x);
		this->m_Rotation.y = yaw;

		// カメラの上昇角度を調整（プレイヤーの前方向と上方向のなす角度）
		pitch = atan2f(-playerForward.y, sqrtf(playerForward.x * playerForward.x + playerForward.z * playerForward.z));
		this->m_Rotation.x = pitch;

		break;
	}//switch
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