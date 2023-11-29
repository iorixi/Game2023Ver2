#pragma once

#include <Windows.h>
#include <iostream>
#include "component.h"

namespace Timer
{

	class Time : public Component
	{

	protected:
		DWORD startTime;//���Ԍv���J�n���̎���
		double nowTime;//���݂̎���

	public:
		using Component::Component;

		virtual void Init() override;
		virtual void Update() override;

		void SetNowStartTime();

	};

}
