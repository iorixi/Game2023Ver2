#include "Delay.h"

using namespace Timer;

void DelayCompnent::Init()
{
	startTime = timeGetTime();//�������Ɏ��Ԃ�����
}

void DelayCompnent::Update()
{
	Time::Update();
}

bool DelayCompnent::DelayProgrees()
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

void DelayCompnent::SetdelayTime(float _delayTime)
{
	delayTime = _delayTime;
}

void DelayCompnent::SetLoop(bool _loop)
{
	loop = _loop;
}
