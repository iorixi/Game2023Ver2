#include "Time.h"

using namespace Timer;

void Time::Init()
{
	
}

void Time::Update()
{
	//¡‚ÌŠÔ‚ğ’²‚×‚é
	DWORD now = timeGetTime();

	//ƒ~ƒŠ•b‚©‚ç•b”‚É•ÏŠ·
	nowTime = (double)(now - startTime) / 1000;

}


void Time::SetNowStartTime()
{
	//¡‚ÌŠÔ‚ğ•Û‘¶
	startTime = timeGetTime();
}


