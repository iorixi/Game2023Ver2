#pragma once
#include "component.h"
#include "renderer.h"
#include "Delay.h"

/// <summary>
/// �v���C���[�̉���s��
/// </summary>

namespace Player
{
	class Evasive : public Component
	{
	public:
		using Component::Component;

		virtual void Init() override;
		virtual void Update() override;
	};
}
