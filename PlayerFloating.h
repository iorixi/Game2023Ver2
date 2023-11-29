#pragma once
#include "component.h"
#include "renderer.h"

namespace Player
{
	class Floating : public Component
	{

		//�㉺�̈ړ��X�s�[�h
		float m_FloatingMoveSpeed = 0.20f;
		//�ҋ@��Ԃ̎��̏㉺�ړ��ő勗��
		float m_StandFloatingDistanceMax = 0.009f;
		//�ҋ@��Ԃ̎��̏㉺�ړ��ő勗��
		float m_StandFloatingDistance = 0.0f;
		//�ҋ@��Ԃ̕������㏸���Ă邩
		bool m_StandRise = true;
		//�ҋ@��Ԃ̏㏸���~���x
		float m_StandSpeed = 0.0007f;

	public:
		using Component::Component;

		virtual void Init() override;
		virtual void Update() override;


	};
}
