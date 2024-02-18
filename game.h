#pragma once

#include "scene.h"
#include <memory>
#include "ScheduledTask.h"
#include <vector>
namespace Sound { class Audio; }

class Sprite;
class Score;

class Game : public Scene
{
private:
	class Transition* m_Transition{};

	bool m_Goal = false;
	bool StartSetTime = false;
	bool m_Go = false;
	bool m_GoEnd = false;
	bool m_titleFlg = false;

	std::shared_ptr<Timer::ScheduledTask> scheduledTask;
	GameObject* Ready;
	GameObject* Go;
	std::vector<Score*> scoreObj;

	std::shared_ptr <Timer::ScheduledTask> m_GameOverTask;
	bool flg = true;
	bool endflg = false;
	bool imguiEndflg = false;

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

private:
	void ReadyGo();
	void Imgui();
};