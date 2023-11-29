#pragma once
#include "Time.h"
#include "component.h"


/// <summary>
/// 
/// ���Ԃ�x��������N���X
/// 
/// </summary>

namespace Timer
{

	class Delay : public Time
	{

		bool delayProgress = false;//�w�莞�Ԃ��o�߂�����
		bool loop = false;//���[�v�����邩
		float delayTime = 0.0f;//�x������

	public:

		using Time::Time;

		void Init() override;
		void Update() override;

		/// <summary>
		/// �o�ߎ��Ԃ��o�߂������ǂ���
		/// �߂�l�F�ݒ肵�����Ԃɓ��B������
		/// </summary>
		bool DelayProgrees();

		/// <summary>
		/// �x�����Ԃ����
		/// �����F�x������
		/// </summary>
		void SetdelayTime(float _delayTime);

		/// <summary>
		/// ���[�v�����邩
		/// �����F���[�v�@�\������ɂ��邩�i���b�ԂɈ��Ȃǂ͂���j
		/// </summary>
		void SetLoop(bool _loop);


	private:

	};
}

