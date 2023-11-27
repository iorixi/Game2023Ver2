#pragma once

#include "gameObject.h"

class Bullet : public GameObject
{

private:

	DirectX::SimpleMath::Vector3		m_Velocity{};

public:
	static void Load();
	static void Unload();

	void Init() override;
	void Update() override;

	void SetVelocity(DirectX::SimpleMath::Vector3 Velocity) { m_Velocity = Velocity; }

};