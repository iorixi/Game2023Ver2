#pragma once
#include "component.h"
#include "renderer.h"

namespace Player
{
	class Floating : public Component
	{

		//上下の移動スピード
		float m_FloatingMoveSpeed = 0.20f;
		//待機状態の時の上下移動最大距離
		float m_StandFloatingDistanceMax = 0.009f;
		//待機状態の時の上下移動最大距離
		float m_StandFloatingDistance = 0.0f;
		//待機状態の方向が上昇してるか
		bool m_StandRise = true;
		//待機状態の上昇下降速度
		float m_StandSpeed = 0.0007f;

	public:
		using Component::Component;

		virtual void Init() override;
		virtual void Update() override;


	};
}
