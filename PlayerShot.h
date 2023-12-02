#pragma once
#include "component.h"
#include "renderer.h"
#include <memory>

namespace Timer { class ScheduledTask; }

namespace Player
{
	class Shot : public Component
	{
		bool addShotFlg = false;

		int	m_Frame;
		float	m_BlendRate;
		std::shared_ptr <Timer::ScheduledTask> m_ScheduledTask;

	public:
		using Component::Component;

		virtual void Init() override;
		virtual void Update() override;

		void AddShot();
	};
}
