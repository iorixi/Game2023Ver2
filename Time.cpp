#include "Time.h"

using namespace Timer;

void Time::Init()
{
	
}

void Time::Update()
{
	//���̎��Ԃ𒲂ׂ�
	DWORD now = timeGetTime();

	//�~���b����b���ɕϊ�
	nowTime = (double)(now - startTime) / 1000;

}


void Time::SetNowStartTime()
{
	//���̎��Ԃ�ۑ�
	startTime = timeGetTime();
}


