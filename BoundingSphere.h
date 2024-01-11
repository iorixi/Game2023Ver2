#pragma once

#include "gameObject.h"
#include <vector>

class BoundingSphere : public GameObject
{
private:
	DirectX::SimpleMath::Vector3 m_Center;  // �X�t�B�A�̒��S���W
	float m_Radius;  // �X�t�B�A�̔��a
	DirectX::SimpleMath::Vector3 m_Max;  // AABB�i���ɕ��s�ȋ��E�{�b�N�X�j�̍ő�l
	DirectX::SimpleMath::Vector3 m_Min;  // AABB�̍ŏ��l

	// ���[���h���W�n�ł̈ʒu���v�Z����֐�
	DirectX::SimpleMath::Vector3 CalcWorldPos(DirectX::SimpleMath::Matrix mtx);

public:
	BoundingSphere();
	BoundingSphere(float m_Radius, DirectX::SimpleMath::Vector3 m_Center);

	// �^����ꂽ���_����o�E���f�B���O�X�t�B�A���v�Z����֐�
	void Caliculate(const std::vector<VERTEX_3D>& vertices);

	// �X�t�B�A�̔��a���擾����֐�
	float GetRadius();

	// �X�t�B�A�̒��S���W���擾����֐�
	DirectX::SimpleMath::Vector3 GetCenter();

	// �X�t�B�A�����[���h���W�n�ŃX�P�[�����O����֐�
	BoundingSphere MakeBS(DirectX::SimpleMath::Matrix mtx, DirectX::SimpleMath::Vector3 scale);

	// �X�P�[�����O�ƃ��[���h���W�ϊ��s�񂩂�V�������[���h�ϊ��s����쐬����֐�
	DirectX::SimpleMath::Matrix MakeWorldMtx(DirectX::SimpleMath::Vector3 scale, DirectX::SimpleMath::Matrix wmtx);

	// AABB�̍ő�l���擾����֐�
	DirectX::SimpleMath::Vector3 GetMax();

	// AABB�̍ŏ��l���擾����֐�
	DirectX::SimpleMath::Vector3 GetMin();

	void SetCenter(DirectX::SimpleMath::Vector3 pos) { m_Center = pos; }
	void SetRadius(float radius) { m_Radius = radius; }
};
