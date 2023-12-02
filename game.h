#pragma once

#include "scene.h"
#include "ImguiManager.h"
#include <memory>

class Game : public Scene
{
private:
	class Transition* m_Transition{};
	class Audio* m_BGM{};
	bool m_Goal = false;
	std::shared_ptr<ImguiManager> imguiManager;
	bool imguiUpdateFlg = false;

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
};