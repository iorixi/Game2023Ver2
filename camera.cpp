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
	Vector3 enemyPosition = enemyObject->GetPosition(); // 敵の位置を取得

	// カメラの位置を設定（プレイヤーの後ろに配置）
	this->m_Position = playerPosition - playerForward * m_CameraDistance;

	//カメラの位置を再調整
	playerPosition.y += m_CameraHeight;

	// カメラの高さを調整
	float cameraHeight = 0.0f;

	// プレイヤーが上昇中の場合、上昇量を考慮してカメラの高さを調整
	float playerVelocityY = playerObject->GetVelocity().y;
	if (playerVelocityY > 0.0f)
	{
		cameraHeight += playerVelocityY;
	}

	float heightFactor = 1.0f - (playerPosition.y - enemyPosition.y) / m_CameraDistance;
	cameraHeight -= heightFactor;

	// カメラの高さを調整
	this->m_Position.y = playerPosition.y + cameraHeight;

	// プレイヤーの方を注視点に設定
	this->m_Target = playerPosition;

	// カメラの回転を調整（プレイヤーの前方向に対して適切な角度）
	float yaw = atan2f(playerForward.z, playerForward.x);
	this->m_Rotation.y = yaw;

	// カメラの上昇角度を調整（プレイヤーの前方向と上方向のなす角度）
	float pitch = atan2f(-playerForward.y, sqrtf(playerForward.x * playerForward.x + playerForward.z * playerForward.z));
	this->m_Rotation.x = pitch;
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