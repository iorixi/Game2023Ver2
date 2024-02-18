#pragma once
#include "component.h"
#include "renderer.h"
#include <memory>
#include "EnemyShootModo.h"

namespace Timer { class ScheduledTask; }

namespace Enemy
{
	class Shot : public Component
	{
		bool addShotFlg = false;

		int	m_Frame;
		float	m_BlendRate;
		std::shared_ptr <Timer::ScheduledTask> m_ScheduledTask;
		std::shared_ptr <Timer::ScheduledTask> m_SpellBulletRate;

		//���̃X�|�[���ꏊ�ɂǂꂮ�炢
		float AddForwardEnemyShotSpawnPos = 4.5f;
		ENEMYSHOOTMODO enemyShootModo;

	public:
		using Component::Component;

		virtual void Init() override;
		virtual void Update() override;
		void AddShot();
	};
}