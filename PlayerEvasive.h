#pragma once
#include "component.h"
#include "renderer.h"
#include "Delay.h"
#include "EvasiveModo.h"

/// <summary>
/// �v���C���[�̉���s��
/// </summary>

//�O���錾
namespace Timer { class ScheduledTask; }

namespace Player
{
	class Evasive : public Component
	{
		bool m_InitFlgEvasiveTime = true;
		bool m_InitFlgEvasiveCoolTime = true;

		float evasiveTime = 0.2f;
		float coolDown = 0.1f;

		float m_EvasiveMoveSpeed = 0;

		DirectX::SimpleMath::Matrix viewMatrix;
		DirectX::SimpleMath::Vector3 cameraForward;//�J�����̌���
		DirectX::SimpleMath::Vector3 playerPosition;//�v���C���[�̒u
		DirectX::SimpleMath::Vector3 playerRotation;//�v���C���[�̉�]

		// �O���x�N�g�����擾
		DirectX::SimpleMath::Vector3 playerForward;

		// �����x
		float acceleration = 1.1f;

		// �v���C���[�ƓG�̊Ԃ̃x�N�g��
		DirectX::SimpleMath::Vector3 playerToEnemy;
		// �v���C���[�̉������x�N�g���i�E�����j
		DirectX::SimpleMath::Vector3 upVector;
		// �ڐ��x�N�g���i�E�����j
		DirectX::SimpleMath::Vector3 tangent;
		//�������̃x�N�g��
		DirectX::SimpleMath::Vector3 horizontalVec;
		float direction = 1;

		EvasiveModo evasiveModo;
		EvasiveVectol evasiveVectol;//������

		std::shared_ptr <Timer::ScheduledTask> m_EvasiveTime;

	public:
		using Component::Component;

		virtual void Init() override;
		virtual void Update() override;

	private:
		//�������
		void EvasiveMove();

		//�����������߂�
		void EvasiveDirection();
	};
}
