#pragma once

#include "gameObject.h"

namespace Enemy
{
	class EnemyObject : public GameObject
	{
	protected:
		int hp = 100;

	public:
		void Init() override;
		virtual void Uninit() override;
		void Damege(int damege);
		int GetHp();
	};
}
