#pragma once

#include "gameObject.h"
#include "animationModel.h"

class Player : public GameObject
{

private:

	DirectX::SimpleMath::Vector3		m_Velocity{};
	class Audio*	m_SE{};

	class AnimationModel* m_Model;
	int	m_Frame;
	float	m_BlendRate;

public:
	void Init() override;
	void Update() override;
	void PreDraw() override;

};