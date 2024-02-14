#pragma once
#include	<SimpleMath.h>
#include	"../renderer.h"
#include "../collision.h"

// OBB
class CBoundingBox {
	BoundingBoxOBB	m_obbinfo;			// OBB���

	// ���[���h���W���v�Z����i�ΏۃI�u�W�F�N�g�̍s��iRTS�j���������j
	DirectX::SimpleMath::Vector3 CalcWorldPos(DirectX::SimpleMath::Matrix worldmtx);

public:
	// ���[�J�����W�Ő�������OBB�����[���h��Ԃɕϊ�
	BoundingBoxOBB MakeOBB(
		DirectX::SimpleMath::Matrix worldmtx,			// �ΏۃI�u�W�F�N�g�̃��[���h�ϊ��s��
		DirectX::SimpleMath::Vector3 scale);			// �g�k��

	// �������Ɏˉe�����e�̏d�Ȃ���`�F�b�N����
	static bool CompareLength(
		const BoundingBoxOBB& ObbA,								// OBB-A
		const BoundingBoxOBB& ObbB,								// OBB-B
		const DirectX::SimpleMath::Vector3& vecSeparate,		// ������
		const DirectX::SimpleMath::Vector3& vecDistance);		// ���S���W�����񂾃x�N�g��

	// AABB���𐶐�����
	void Caliculate(const std::vector<VERTEX_3D>& vertices);

	// ���S���W��߂�
	DirectX::SimpleMath::Vector3 GetCenter();

	// BBOX�`��p�̃��[���h�ϊ��s��𐶐�����
	DirectX::SimpleMath::Matrix MakeWorldMtx(
		DirectX::SimpleMath::Vector3 scale,
		DirectX::SimpleMath::Matrix wmtx);

	// �ő�l�擾
	DirectX::SimpleMath::Vector3 GetMax();

	// �ŏ��l�擾
	DirectX::SimpleMath::Vector3 GetMin();

	// OBB�擾
	BoundingBoxOBB GetOBB();
};