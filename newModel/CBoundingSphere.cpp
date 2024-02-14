#include	"CBoundingSphere.h"
using namespace DirectX::SimpleMath;
#undef max//�Փˉ��

// ���[���h���W���v�Z����i�ΏۃI�u�W�F�N�g�̍s��iRTS�j���������j
Vector3 CBoundingSphere::CalcWorldPos(DirectX::SimpleMath::Matrix mtx) {
	return Vector3::Transform(m_center, mtx);
}

// BS���𐶐�����
void CBoundingSphere::Caliculate(const std::vector<VERTEX_3D>& vertices) {
	std::vector<float > lengthlist;

	DirectX::SimpleMath::Vector3 max;
	DirectX::SimpleMath::Vector3 min;

	// �ő�l�ƍŏ��l�����߂�
	min = max = vertices[0].Position;
	for (auto& v : vertices) {
		if (min.x > v.Position.x) min.x = v.Position.x;
		if (min.y > v.Position.y) min.y = v.Position.y;
		if (min.z > v.Position.z) min.z = v.Position.z;

		if (max.x < v.Position.x) max.x = v.Position.x;
		if (max.y < v.Position.y) max.y = v.Position.y;
		if (max.z < v.Position.z) max.z = v.Position.z;
	}

	m_min = min;
	m_max = max;

	std::cout << m_min.x << "," << m_min.y << "," << m_min.z << std::endl;
	std::cout << m_max.x << "," << m_max.y << "," << m_max.z << std::endl;

	// ���S�����߂�
	m_center = (max + min) * 0.5f;

	// ���a�����߂�
	for (auto& v : vertices) {
		float length;
		length = (v.Position - m_center).Length();
		lengthlist.emplace_back(length);
	}

	// �ő�̒��������߂�
	auto iter = std::max_element(lengthlist.begin(), lengthlist.end());
	m_radius = *iter;
}

// ���a��߂�
float CBoundingSphere::GetRadius() {
	return m_radius;
}

// ���S���W��߂�
Vector3 CBoundingSphere::GetCenter() {
	return m_center;
}

// BS��񐶐�
BoundingSphere CBoundingSphere::MakeBS(
	DirectX::SimpleMath::Matrix mtx,			// �I�u�W�F�N�g�̍s��iRTS���j
	DirectX::SimpleMath::Vector3 scale)			// �I�u�W�F�N�g�̃X�P�[���l
{
	BoundingSphere bs;

	// ���S���W�v�Z
	Vector3 wpos = CalcWorldPos(mtx);
	bs.center = wpos;

	// ���a�̍Čv�Z�i�g�k������ׁj
	float maxsclae = std::max(std::max(scale.x, scale.y), scale.z);
	bs.radius = m_radius * maxsclae;

	return bs;
}

// BS�`��p�̃��[���h�ϊ��s��𐶐�����
Matrix CBoundingSphere::MakeWorldMtx(
	Vector3 scale,
	Matrix wmtx) {
	// �P���ɂR�c�W�I���g����񂩂�a�r�����쐬���Ă�
	//�i�g�k���l������Ă��Ȃ��j
	float smax;
	smax = std::max(std::max(scale.x, scale.y), scale.z);

	// �ő�̊g�嗦�ōs�񐶐�
	Matrix mtx = Matrix::CreateScale(smax, smax, smax) * wmtx;
	Vector3 bspos = CalcWorldPos(wmtx);

	// �o�E���f�B���O�X�t�B�A�p�̃��[���h�ϊ��s��ɂ���i���S������Ă�̂ŏC���j
	mtx._41 = bspos.x;
	mtx._42 = bspos.y;
	mtx._43 = bspos.z;

	return mtx;
}

// �ő�l�擾
Vector3 CBoundingSphere::GetMax() {
	return m_max;
}

// �ŏ��l�擾
Vector3 CBoundingSphere::GetMin() {
	return m_min;
}