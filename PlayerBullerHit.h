#pragma once
#include "component.h"

namespace Player
{
	class BulletHit : public Component
	{
	protected:
		//�����������ǂ���
		bool hitFlg = false;

	public:

		using Component::Component;

		virtual void Update() override;

		//�z�[�~���O�e�̓����蔻�菈��
		virtual void HomingHit();
		//�z�[�~���O�e�̓����蔻�菈��
		virtual void SpellOneHit();

		bool GetHitFlg();

	private:
	};
}