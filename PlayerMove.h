#pragma once
#include "component.h"
#include "renderer.h"

namespace Player
{
	class Move : public Component
	{
		//左右の移動スピード
		float m_SlideMoveSpeed = 0.20f;
		//前後の移動スピード
		float m_AroundMoveSpeed = 0.20f;

	public:
		using Component::Component;

		virtual void Init() override;
		virtual void Update() override;
	};
}
