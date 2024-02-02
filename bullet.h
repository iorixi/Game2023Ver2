#pragma once

#include "gameObject.h"
#include <memory>

class BoundingSphereObj;

class Bullet : public GameObject
{
protected:

	DirectX::SimpleMath::Vector3		m_Velocity{};
	bool m_PlayerShot = true;//�v���C���[���ł������ǂ���
	std::shared_ptr<BoundingSphereObj> m_Child;

public:
	static void Load();
	static void Unload();

	virtual void Init() override;
	virtual void Update() override;

	void SetPlayerShot(bool value) { m_PlayerShot = value; };
	bool GetPlayerShot() { return m_PlayerShot; };

	void SetVelocity(DirectX::SimpleMath::Vector3 Velocity) { m_Velocity = Velocity; }
	DirectX::SimpleMath::Vector3 GetVelocity() { return m_Velocity; }
	BoundingSphereObj* GetBulletHitSphere() { return m_Child.get(); };
};