#pragma once

#include "scene.h"
#include <vector>
class Title : public Scene
{
private:
	class Transition* m_Transition{};
	GameObject* selectObj;
	GameObject* titleChara;
	std::vector<GameObject> SelectComand;
	float moveSpeed;
	float targetPosX;
public:
	void Init() override;
	void Update() override;
};