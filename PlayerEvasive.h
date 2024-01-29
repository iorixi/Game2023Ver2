#pragma once
#include "component.h"
#include "renderer.h"
#include "Delay.h"

/// <summary>
/// �v���C���[�̉���s��
/// </summary>

namespace Player
{
	class Evasive : public Component
	{
		float m_EvasiveMoveSpeed = 2.60f;

		DirectX::SimpleMath::Matrix viewMatrix;
		DirectX::SimpleMath::Vector3 cameraForward;//�J�����̌���
		DirectX::SimpleMath::Vector3 playerPosition;//�v���C���[�̒u
		DirectX::SimpleMath::Vector3 playerRotation;//�v���C���[�̉�]

		// �O���x�N�g�����擾
		DirectX::SimpleMath::Vector3 playerForward;

		// �����x
		float acceleration = 0.01f;

		// �v���C���[�ƓG�̊Ԃ̃x�N�g��
		DirectX::SimpleMath::Vector3 playerToEnemy;
		// �v���C���[�̉������x�N�g���i�E�����j
		DirectX::SimpleMath::Vector3 upVector;
		// �ڐ��x�N�g���i�E�����j
		DirectX::SimpleMath::Vector3 tangent;
		//�������̃x�N�g��
		DirectX::SimpleMath::Vector3 horizontalVec;
		float direction = 1;

	public:
		using Component::Component;

		virtual void Init() override;
		virtual void Update() override;
	};
}
