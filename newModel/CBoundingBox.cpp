#include	"CBoundingBox.h"
using namespace DirectX::SimpleMath;

// ���[���h���W���v�Z����i�ΏۃI�u�W�F�N�g�̍s��iRTS�j���������j
Vector3 CBoundingBox::CalcWorldPos(Matrix worldmtx) {
	return Vector3::Transform(m_obbinfo.center, worldmtx);
}

// ���[�J�����W�Ő�������OBB�����[���h��Ԃɕϊ�
BoundingBoxOBB CBoundingBox::MakeOBB(
	Matrix worldmtx,			// �ΏۃI�u�W�F�N�g�̃��[���h�ϊ��s��
	Vector3 scale) {			// �g�k��
	// �o�͗p
	BoundingBoxOBB obb;
	obb = m_obbinfo;

	// ���s�ړ�
	obb.worldcenter = Vector3::Transform(m_obbinfo.center, worldmtx);

	// �ړ������J�b�g
	worldmtx._41 = 0.0f;
	worldmtx._42 = 0.0f;
	worldmtx._43 = 0.0f;

	// ����]
	obb.axisX = DirectX::SimpleMath::Vector3::Transform(m_obbinfo.axisX, worldmtx);
	obb.axisY = DirectX::SimpleMath::Vector3::Transform(m_obbinfo.axisY, worldmtx);
	obb.axisZ = DirectX::SimpleMath::Vector3::Transform(m_obbinfo.axisZ, worldmtx);

	// ���𐳋K��
	obb.axisX.Normalize();
	obb.axisY.Normalize();
	obb.axisZ.Normalize();

	// �g�k���l��
	obb.lengthx *= scale.x;
	obb.lengthy *= scale.y;
	obb.lengthz *= scale.z;

	return obb;
}

// AABB���𐶐�����
void CBoundingBox::Caliculate(const std::vector<VERTEX_3D>& vertices) {
	Vector3 max;
	Vector3 min;

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

	m_obbinfo.min = min;
	m_obbinfo.max = max;

	// ���S�����߂�
	m_obbinfo.worldcenter = m_obbinfo.center = (max + min) * 0.5f;

	// ����ݒ�
	m_obbinfo.axisX = Vector3(1, 0, 0);
	m_obbinfo.axisY = Vector3(0, 1, 0);
	m_obbinfo.axisZ = Vector3(0, 0, 1);

	// ������ݒ�
	DirectX::SimpleMath::Vector3 len;
	len = m_obbinfo.max - m_obbinfo.min;

	m_obbinfo.lengthx = fabs(len.x);
	m_obbinfo.lengthy = fabs(len.y);
	m_obbinfo.lengthz = fabs(len.z);
}

// ���S���W��߂�
Vector3 CBoundingBox::GetCenter() {
	return m_obbinfo.center;
}

// BBOX�`��p�̃��[���h�ϊ��s��𐶐�����
Matrix CBoundingBox::MakeWorldMtx(
	Vector3 scale,
	Matrix wmtx) {
	// �g�k���l��
	Matrix mtx = wmtx;

	// ���S���W���Čv�Z
	Vector3 position = CalcWorldPos(wmtx);

	// �o�E���f�B���O�X�t�B�A�p�̃��[���h�ϊ��s��ɂ���i���S������Ă�̂ŏC���j
	mtx._41 = position.x;
	mtx._42 = position.y;
	mtx._43 = position.z;

	return mtx;
}

// �ő�l�擾
Vector3 CBoundingBox::GetMax() {
	return m_obbinfo.max;
}

// �ŏ��l�擾
Vector3 CBoundingBox::GetMin() {
	return m_obbinfo.min;
}

// OBB�擾
BoundingBoxOBB CBoundingBox::GetOBB() {
	return m_obbinfo;
}

// OBB�̏d�Ȃ�𔻒肷��
bool CBoundingBox::CompareLength(
	const BoundingBoxOBB& ObbA,		// OBB-A
	const BoundingBoxOBB& ObbB,		// OBB-B
	const Vector3& vecSeparate,		// ������
	const Vector3& vecDistance)		// ���S���W�����񂾃x�N�g��
{
	float fDistance{};

	// �������Ɏˉe�������S�Ԃ����ԃx�N�g��
	fDistance = vecDistance.Dot(vecSeparate);	// ���ς��v�Z
	fDistance = fabsf(fDistance);				// �v���X�ɂ���

	//��������Ƀ{�b�N�XA���ˉe�����e�̒���
	float fShadowA = 0;

	//��������Ƀ{�b�N�X�a���ˉe�����e�̒���
	float fShadowB = 0;

	//�{�b�N�XA�́h�e�h���Z�o
	float fShadowAx{};
	float fShadowAy{};
	float fShadowAz{};

	// X���𕪗����Ɏˉe
	fShadowAx = vecSeparate.Dot(ObbA.axisX);
	fShadowAx = fabsf(fShadowAx * (ObbA.lengthx / 2.0f));

	// Y���𕪗����Ɏˉe
	fShadowAy = vecSeparate.Dot(ObbA.axisY);
	fShadowAy = fabsf(fShadowAy * (ObbA.lengthy / 2.0f));

	// Z���𕪗����Ɏˉe
	fShadowAz = vecSeparate.Dot(ObbA.axisZ);
	fShadowAz = fabsf(fShadowAz * (ObbA.lengthz / 2.0f));

	// �������Ɏˉe�����e�̒��������߂�
	fShadowA = fShadowAx + fShadowAy + fShadowAz;

	//�{�b�N�XB�́h�e�h���Z�o
	float fShadowBx{};
	float fShadowBy{};
	float fShadowBz{};

	// X���𕪗����Ɏˉe
	fShadowBx = vecSeparate.Dot(ObbB.axisX);
	fShadowBx = fabsf(fShadowBx * (ObbB.lengthx / 2.0f));

	// Y���𕪗����Ɏˉe
	fShadowBy = vecSeparate.Dot(ObbB.axisY);
	fShadowBy = fabsf(fShadowBy * (ObbB.lengthy / 2.0f));

	// Z���𕪗����Ɏˉe
	fShadowBz = vecSeparate.Dot(ObbB.axisZ);
	fShadowBz = fabsf(fShadowBz * (ObbB.lengthz / 2.0f));

	// �������Ɏˉe�����e�̒��������߂�
	fShadowB = fShadowBx + fShadowBy + fShadowBz;

	if (fDistance > fShadowA + fShadowB) {
		return false;
	}

	return true;
}