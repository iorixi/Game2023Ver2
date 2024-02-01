#include "BoundingSphere.h"
#include <algorithm>
#include <vector>

using namespace DirectX::SimpleMath;

Vector3 BoundingSphereObj::CalcWorldPos(Matrix mtx) {
	// ���[���h���W���v�Z����
	return Vector3::Transform(m_Center, mtx);
}

BoundingSphereObj::BoundingSphereObj()
{
}

BoundingSphereObj::BoundingSphereObj(float radius, DirectX::SimpleMath::Vector3 center)
{
	m_Center = center;
	m_Radius = radius;
}

void BoundingSphereObj::Caliculate(const std::vector<VERTEX_3D>& vertices) {
	std::vector<float> lengthlist;

	Vector3 max;
	Vector3 min;
	min = max = vertices[0].Position;

	// AABB�̍ő�ƍŏ����v�Z����
	for (auto& v : vertices) {
		if (min.x > v.Position.x) min.x = v.Position.x;
		if (min.y > v.Position.y) min.y = v.Position.y;
		if (min.z > v.Position.z) min.z = v.Position.z;

		if (max.x < v.Position.x) max.x = v.Position.x;
		if (max.y < v.Position.y) max.y = v.Position.y;
		if (max.z < v.Position.z) max.z = v.Position.z;
	}

	m_Min = min;
	m_Max = max;

	// ���S���W���v�Z����
	m_Center = (max + min) * 0.5f;

	for (auto& v : vertices) {
		// ���_���璆�S�܂ł̋������v�Z���A���X�g�ɒǉ�����
		float length;
		length = (v.Position - m_Center).Length();
		lengthlist.emplace_back(length);
	}

	// ���X�g���̍ő�̒����𔼌a�Ƃ���
	auto iter = std::max_element(lengthlist.begin(), lengthlist.end());
	m_Radius = *iter;
}

float BoundingSphereObj::GetRadius() {
	// ���a���擾����
	return m_Radius;
}

Vector3 BoundingSphereObj::GetCenter() {
	// ���S���W���擾����
	return m_Center;
}

BoundingSphereObj BoundingSphereObj::MakeBS(Matrix mtx, Vector3 scale) {
	BoundingSphereObj bs;

	// ���S���W�����[���h���W�ɕϊ����Đݒ肷��
	Vector3 wpos = CalcWorldPos(mtx);
	bs.m_Center = wpos;

	// �X�P�[���̍ő�l�����߁A���a���Čv�Z����
	float maxsclae = (std::max)((std::max)(scale.x, scale.y), scale.z);
	bs.m_Radius = m_Radius * maxsclae;

	return bs;
}

Matrix BoundingSphereObj::MakeWorldMtx(Vector3 scale, Matrix wmtx) {
	// �X�P�[���̍ő�l�����߁A���[���h�ϊ��s����쐬����
	float smax;
	smax = (std::max)((std::max)(scale.x, scale.y), scale.z);

	Matrix mtx = Matrix::CreateScale(smax, smax, smax) * wmtx;
	Vector3 bspos = CalcWorldPos(wmtx);

	// �o�E���f�B���O�X�t�B�A�p�̃��[���h�ϊ��s��ɂ���i���S������Ă���̂ŏC���j
	mtx._41 = bspos.x;
	mtx._42 = bspos.y;
	mtx._43 = bspos.z;

	return mtx;
}

Vector3 BoundingSphereObj::GetMax() {
	// AABB�̍ő�l���擾����
	return m_Max;
}

Vector3 BoundingSphereObj::GetMin() {
	// AABB�̍ŏ��l���擾����
	return m_Min;
}