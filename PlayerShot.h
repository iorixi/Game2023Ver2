#pragma once
#include "component.h"
#include "renderer.h"
#include <memory>
#include "PlayerShootModo.h"

namespace Timer { class ScheduledTask; }

namespace Player
{
	class Shot : public Component
	{
		bool addShotFlg = false;

		int	m_Frame;
		float	m_BlendRate;
		std::shared_ptr <Timer::ScheduledTask> m_ScheduledTask;

		//���̃X�|�[���ꏊ�ɂǂꂮ�炢
		float AddForwardPlayerShotSpawnPos = 4.5f;
		PlayerShootModo playerShootModo;

	public:
		using Component::Component;

		virtual void Init() override;
		virtual void Update() override;
		void AddShot();
	};
}
