#include "Delay.h"

using namespace Timer;

void DelayCompnent::Init()
{
	startTime = timeGetTime();//生成時に時間を入れる
}

void DelayCompnent::Update()
{
	Time::Update();
}

bool DelayCompnent::DelayProgrees()
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

void DelayCompnent::SetdelayTime(float _delayTime)
{
	delayTime = _delayTime;
}

void DelayCompnent::SetLoop(bool _loop)
{
	loop = _loop;
}
