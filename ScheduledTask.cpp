#include "ScheduledTask.h"
#include <thread>
#include <iostream>

using namespace Timer;

// �f�t�H���g�R���X�g���N�^
ScheduledTask::ScheduledTask()
{
}

// �R���X�g���N�^�̎���
ScheduledTask::ScheduledTask(float interval) : m_Interval(static_cast<long long>(interval * 1000.0)), m_Stopped(false) {
	// �ŏ��̎��s���Ԃ�ݒ�
	m_ScheduledTime = std::chrono::system_clock::now() + m_Interval;
}

// �^�C�}�[�̐ݒ�
void ScheduledTask::SetTimer(float interval) {
	// interval���~���b�ɕϊ����ă^�C�}�[�̐ݒ�
	m_Interval = std::chrono::milliseconds(static_cast<long long>(interval * 1000.0));
	m_ScheduledTime = std::chrono::system_clock::now() + m_Interval;
}

// �w�肵�����Ԃ��o�߂�����true��Ԃ��A�����Ƀt���O��false�ɂ���
bool ScheduledTask::GetFlg() {
	if (m_Stopped) {
		return false;  // �o�ߎ��Ԃ���~���͏��false��Ԃ�
	}

	// ���݂̎��Ԃ��擾
	auto currentTime = std::chrono::system_clock::now();

	// �w�肵�����ԂɂȂ�����true�ɂ���
	if (currentTime > m_ScheduledTime) {
		// �t���O��false�ɂ��āA����̎��s���Ԃ�ݒ�
		m_ScheduledTime = std::chrono::system_clock::now() + m_Interval;
		return true;
	}

	return false;
}

// �o�ߎ��Ԃ��~
void ScheduledTask::Stop() {
	if (!m_Stopped) {
		m_PausedDuration = std::chrono::system_clock::now() - m_ScheduledTime;
		m_Stopped = true;
	}
}

// ��~���Ă����o�ߎ��Ԃ�i�߂�
void ScheduledTask::Start() {
	if (m_Stopped) {
		m_ScheduledTime = std::chrono::system_clock::now() + m_PausedDuration;
		m_Stopped = false;
	}
}

// �o�ߎ��Ԃ�0�Ƀ��Z�b�g���čăX�^�[�g
void ScheduledTask::Restart() {
	m_ScheduledTime = std::chrono::system_clock::now() + m_Interval;
	m_Stopped = false;
}

/*
  0.8�b���ƂɃ^�X�N�����s����ScheduledTask���쐬
  ScheduledTask scheduledTask(0.8f);

  // Get���\�b�h���ĂԂ��ƂŎw�肵�����Ԃ��o�߂���܂őҋ@���A���̌��true���擾
  bool result = scheduledTask.GetFlg();

  if (result) {
	  // �w�肵�����Ԃ��o�߂������̏����������ɏ���
  }
  else {
	  // �G���[�������������ɏ���
  }

  // �o�ߎ��Ԃ��~
  scheduledTask.Stop();

  // �o�ߎ��Ԃ�i�߂�
  scheduledTask.Start();
*/