#pragma once
#include "bullet.h"
#include "renderer.h"
#include <memory>

namespace Timer { class ScheduledTask; }

class CircleSkillShoot : Bullet
{
	std::unique_ptr <Timer::ScheduledTask> m_ShootTime;

public:
	CircleSkillShoot();
	~CircleSkillShoot();

	static void Load();
	static void Unload();

	void Init() override;
	void Update() override;

private:
};
