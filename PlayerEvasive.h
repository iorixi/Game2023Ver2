#pragma once
#include "component.h"
#include "renderer.h"
#include "Delay.h"
#include "EvasiveModo.h"

/// <summary>
/// プレイヤーの回避行動
/// </summary>

//前方宣言
namespace Timer { class ScheduledTask; }

namespace Player
{
	class Evasive : public Component
	{
		bool m_InitFlgEvasiveTime = true;
		bool m_InitFlgEvasiveCoolTime = true;

		float evasiveTime = 0.2f;
		float coolDown = 0.1f;

		float m_EvasiveMoveSpeed = 0;

		DirectX::SimpleMath::Matrix viewMatrix;
		DirectX::SimpleMath::Vector3 cameraForward;//カメラの向き
		DirectX::SimpleMath::Vector3 playerPosition;//プレイヤーの置
		DirectX::SimpleMath::Vector3 playerRotation;//プレイヤーの回転

		// 前方ベクトルを取得
		DirectX::SimpleMath::Vector3 playerForward;

		// 加速度
		float acceleration = 1.1f;

		// プレイヤーと敵の間のベクトル
		DirectX::SimpleMath::Vector3 playerToEnemy;
		// プレイヤーの横向きベクトル（右向き）
		DirectX::SimpleMath::Vector3 upVector;
		// 接線ベクトル（右向き）
		DirectX::SimpleMath::Vector3 tangent;
		//横向きのベクトル
		DirectX::SimpleMath::Vector3 horizontalVec;
		float direction = 1;

		EvasiveModo evasiveModo;
		EvasiveVectol evasiveVectol;//回避方向

		std::shared_ptr <Timer::ScheduledTask> m_EvasiveTime;

	public:
		using Component::Component;

		virtual void Init() override;
		virtual void Update() override;

	private:
		//回避処理
		void EvasiveMove();

		//回避方向を決める
		void EvasiveDirection();
	};
}
