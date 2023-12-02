#pragma once
#include "component.h"
#include "renderer.h"

namespace Player
{
	class Move : public Component
	{
		//���E�̈ړ��X�s�[�h
		float m_SlideMoveSpeed = 0.20f;
		//�O��̈ړ��X�s�[�h
		float m_AroundMoveSpeed = 0.20f;

	public:
		using Component::Component;

		virtual void Init() override;
		virtual void Update() override;
	};
}
