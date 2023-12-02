#pragma once

#include <chrono>

namespace Timer
{
	/// <summary>
	/// �w�肵�����Ԃ��ƂɃ^�X�N�����s���邽�߂̃N���X
	/// </summary>
	class ScheduledTask {
		// �^�X�N�̎��s�Ԋu���~���b�P�ʂŕێ�
		std::chrono::milliseconds m_Interval;

		// ����̎��s����
		std::chrono::system_clock::time_point m_ScheduledTime;

		// �^�X�N�̎��s���~���Ă��邩�ǂ����̃t���O
		bool m_Stopped;

		// �^�X�N����~���Ă�������
		std::chrono::system_clock::duration m_PausedDuration;

	public:

		// �f�t�H���g�R���X�g���N�^
		ScheduledTask();

		// �R���X�g���N�^�F�w�肵���Ԋu�Ń^�X�N�����s����ScheduledTask���쐬
		ScheduledTask(float interval);

		// �^�C�}�[�̏�����
		void SetTimer(float interval);

		// �w�肵�����Ԃ��o�߂�����true��Ԃ��A�����Ƀt���O��false�ɂ���
		bool GetFlg();

		// �o�ߎ��Ԃ��~
		void Stop();

		// ��~���Ă����o�ߎ��Ԃ�i�߂�
		void Start();

		// �o�ߎ��Ԃ�0�Ƀ��Z�b�g���čăX�^�[�g
		void Restart();

	private:
	};
}