#pragma once

#include <Windows.h>
#include <iostream>
#include "component.h"

namespace Timer
{

	class Time : public Component
	{

	protected:
		DWORD startTime;//ŠÔŒv‘ªŠJn‚ÌŠÔ
		double nowTime;//Œ»İ‚ÌŠÔ

	public:
		using Component::Component;

		virtual void Init() override;
		virtual void Update() override;

		void SetNowStartTime();

	};

}
