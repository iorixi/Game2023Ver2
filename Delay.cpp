#include "Delay.h"

using namespace Timer;

void Delay::Init()
{
	startTime = timeGetTime();//�������Ɏ��Ԃ�����
}

void Delay::Update()
{
	Time::Update();
}

bool Delay::DelayProgrees()
{
	//�o�ߎ��Ԃ��ݒ肵�����Ԃ𒴂�����
	if (nowTime > delayTime)
	{
		//���Z�b�g����Ȃ�
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
