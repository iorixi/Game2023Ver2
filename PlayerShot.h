#pragma once
#include "component.h"
#include "renderer.h"

namespace Timer { class DelayCompnent; }

namespace Player
{

	class Shot : public Component
	{
		bool addShotFlg = false;

		int	m_Frame;
		float	m_BlendRate;
		Timer::DelayCompnent* m_Delay{};

	public:
		using Component::Component;

		virtual void Init() override;
		virtual void Update() override;

		void AddShot();
	};

}
