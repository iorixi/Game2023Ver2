#pragma once

#include	<simplemath.h>

// �~����`
struct BoundingCylinder {
	DirectX::SimpleMath::Vector3    bottom;						// ��
	DirectX::SimpleMath::Vector3    top;						// ��
	float	radius;												// ���a
};

// ����`
struct BoundingSphere {
	DirectX::SimpleMath::Vector3	center;
	float	radius;
};

// �Z�O�����g�i�����j
struct Segment {
	DirectX::SimpleMath::Vector3		startpoint;				// �J�n�_
	DirectX::SimpleMath::Vector3		endpoint;				// �I���_
};

// BOX��`
struct AABB {
	DirectX::SimpleMath::Vector3	min;
	DirectX::SimpleMath::Vector3	max;
};

// ��
bool CollisionSphere(BoundingSphere p1, BoundingSphere p2);

// AABB
bool CollisionAABB(AABB p1, AABB p2);

// make AABB
AABB SetAABB(DirectX::SimpleMath::Vector3 centerposition, float width, int height, int depth);

// �����Ɠ_�̋��������߂�
float calcPointLineDist(
	const DirectX::SimpleMath::Vector3& point,
	const Segment& segment,
	DirectX::SimpleMath::Vector3& intersectionpoint,
	float& t);

// �����ɕ��s�ȗ^����ꂽ���̂S�p�`�̍��W�����߂�
void CalcQuadOrientedLine(
	DirectX::SimpleMath::Vector3 startpos,
	DirectX::SimpleMath::Vector3 endpos,
	DirectX::SimpleMath::Vector3* v,
	float width);

// �����Ɠ_�̋��������߂�
float calcPointLineDist(
	const DirectX::SimpleMath::Vector3& point,
	const Segment& segment,
	DirectX::SimpleMath::Vector3& intersectionpoint,
	float& t);

// �����ƒ����̒��������߂�
float calcPointSegmentDist(
	const DirectX::SimpleMath::Vector3& p,
	const Segment& segment,
	DirectX::SimpleMath::Vector3& intersectionpoint,
	float& t);

// ���������������i�l�p�`�j�Ƌ��̓����蔻����s��
bool CollisionSphereOrientedQuad(
	DirectX::SimpleMath::Vector3 StartPos,						// �J�n�ʒu
	DirectX::SimpleMath::Vector3 EndPos,							// �I���ʒu
	float width,							// ��
	float radius,							// ���a
	DirectX::SimpleMath::Vector3 CirclePos);						// ���S���W

// �܂������ɗ����Ă�~���Ƌ��̓����蔻��
bool CollisionSphereCylinder(BoundingSphere sphere, BoundingCylinder cylinder);

// �O�p�`�̓������ǂ����𔻒肷��
bool CheckInTriangle(
	const DirectX::SimpleMath::Vector3& a,
	const DirectX::SimpleMath::Vector3& b,
	const DirectX::SimpleMath::Vector3& c,
	const DirectX::SimpleMath::Vector3& p);

bool LinetoPlaneCross(
	const DirectX::SimpleMath::Plane& plane,		//���ʂ̕�����
	const DirectX::SimpleMath::Vector3& p0,			//�����̋N�_
	const DirectX::SimpleMath::Vector3& wv,			//�����̕����x�N�g��
	float& t,										// ��_�ʒu���
	DirectX::SimpleMath::Vector3& ans);				//��_���W
