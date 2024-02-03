#pragma once
#include "component.h"
#include "renderer.h"
#include "MoveModo.h"

namespace Enemy
{
	class Move : public Component
	{
		//���E�̈ړ��X�s�[�h
		float m_SlideMoveSpeed = 0.20f;
		//�O��̈ړ��X�s�[�h
		float m_AroundMoveSpeed = 0.20f;

		MoveModo moveModo;
		//�ړ����̃x�N�g��
		DirectX::SimpleMath::Vector3 moveVec;

		DirectX::SimpleMath::Matrix viewMatrix;
		DirectX::SimpleMath::Vector3 cameraForward;//�J�����̌���
		DirectX::SimpleMath::Vector3 enemyPosition;//�v���C���[�̒u
		DirectX::SimpleMath::Vector3 enemyRotation;//�v���C���[�̉�]

		// �O���x�N�g�����擾
		DirectX::SimpleMath::Vector3 enemyForward;

		// �����x
		float acceleration = 0.01f;

		// �v���C���[�ƓG�̊Ԃ̃x�N�g��
		DirectX::SimpleMath::Vector3 enemyToPlayer;
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
		MoveModo GetEnemyMoveModo();

	private:
		void FarDistance();
		void CloseDistance();
		DirectX::SimpleMath::Vector3 GetMoveVec();
	};
}
