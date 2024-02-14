#pragma once
#include "component.h"

namespace Player
{
	class BulletHit : public Component
	{
	protected:
		//当たったかどうか
		bool hitFlg = false;

	public:

		using Component::Component;

		virtual void Update() override;

		//ホーミング弾の当たり判定処理
		virtual void HomingHit();

		bool GetHitFlg();

	private:
	};
}