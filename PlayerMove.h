#pragma once
#include "component.h"
#include "renderer.h"
#include "MoveModo.h"

namespace Player
{
	class Move : public Component
	{
		//左右の移動スピード
		float m_SlideMoveSpeed = 0.20f;
		//前後の移動スピード
		float m_AroundMoveSpeed = 0.20f;

		MoveModo moveModo;

		DirectX::SimpleMath::Matrix viewMatrix;
		DirectX::SimpleMath::Vector3 cameraForward;//カメラの向き
		DirectX::SimpleMath::Vector3 playerPosition;//プレイヤーの置
		DirectX::SimpleMath::Vector3 playerRotation;//プレイヤーの回転

		// 前方ベクトルを取得
		DirectX::SimpleMath::Vector3 playerForward;

		// 加速度
		float acceleration = 0.01f;

		// プレイヤーと敵の間のベクトル
		DirectX::SimpleMath::Vector3 playerToEnemy;
		// プレイヤーの横向きベクトル（右向き）
		DirectX::SimpleMath::Vector3 upVector;
		// 接線ベクトル（右向き）
		DirectX::SimpleMath::Vector3 tangent;
		//横向きのベクトル
		DirectX::SimpleMath::Vector3 horizontalVec;
		float direction = 1;

	public:
		using Component::Component;

		virtual void Init() override;
		virtual void Update() override;
		MoveModo GetPlayerMoveModo();

	private:
		void FarDistance();
		void CloseDistance();
	};
}
