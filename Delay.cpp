#include "Delay.h"

using namespace Timer;

void Delay::Init()
{
	startTime = timeGetTime();//生成時に時間を入れる
}

void Delay::Update()
{
	Time::Update();
}

bool Delay::DelayProgrees()
{
	//経過時間が設定した時間を超えたら
	if (nowTime > delayTime)
	{
		//リセットするなら
		if (loop)
		{

			SetNowStartTime();

		}
		return true;

	}
	return false;
}

void Delay::SetdelayTime(float _delayTime)
{
	delayTime = _delayTime;
}

void Delay::SetLoop(bool _loop)
{
	loop = _loop;
}
