#pragma once
#include "bullet.h"

class HomingBullet : public Bullet
{
public:
	HomingBullet();
	~HomingBullet();

	static void Load();
	static void Unload();

	void Init() override;
	void Update() override;

private:
};
