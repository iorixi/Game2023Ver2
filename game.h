#pragma once

#include "scene.h"
#include <memory>
#include "ScheduledTask.h"

class Game : public Scene
{
private:
	class Transition* m_Transition{};
	class Audio* m_BGM{};

	bool m_Goal = false;
	bool StartSetTime = false;
	bool m_Go = false;
	bool m_GoEnd = false;
	bool m_titleFlg = false;

	std::shared_ptr<Timer::ScheduledTask> scheduledTask;
	GameObject* Ready;
	GameObject* Go;

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

private:
	void ReadyGo();
	void Imgui();
};