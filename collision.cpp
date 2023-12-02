#include	"collision.h"

using namespace DirectX::SimpleMath;

// �����ɕ��s�ȗ^����ꂽ���̂S�p�`�̍��W�����߂�
void CalcQuadOrientedLine(
	DirectX::SimpleMath::Vector3 startpos,
	DirectX::SimpleMath::Vector3 endpos,
	DirectX::SimpleMath::Vector3* v,
	float width) {
	// �����ȃx�N�g�������߂�
	Vector3 zaxis(0, 0, 1);
	Vector3 line = endpos - startpos;

	Vector3 ans;
	ans = line.Cross(zaxis);					// �O��
	ans.Normalize();							// ���K��

	Vector3 startposplus = startpos + ans * width / 2.0f;
	Vector3 startposminus = startpos - ans * width / 2.0f;

	Vector3 endposplus = endpos + ans * width / 2.0f;
	Vector3 endposminus = endpos - ans * width / 2.0f;

	v[0] = startposplus;
	v[1] = endposplus;

	v[2] = startposminus;
	v[3] = endposminus;
}

// �����Ɠ_�̋��������߂�
float calcPointLineDist(
	const Vector3& point,
	const Segment& segment,
	Vector3& intersectionpoint,
	float& t) {
	float distance = 0.0f;

	// �����̃x�N�g�������߂�
	double ABx = segment.endpoint.x - segment.startpoint.x;
	double ABy = segment.endpoint.y - segment.startpoint.y;
	double ABz = segment.endpoint.z - segment.startpoint.z;

	// �����̎n�_�Ɠ_�����ԃx�N�g�������߂�
	double APx = point.x - segment.startpoint.x;
	double APy = point.y - segment.startpoint.y;
	double APz = point.z - segment.startpoint.z;

	// ����AB�̒��������߂�
	double AB2 = ABx * ABx + ABy * ABy + ABz * ABz;

	// AB�x�N�g����AP�x�N�g���̓��ς��v�Z����i�ˉe�������������߂�j
	double ABdotAP = ABx * APx + ABy * APy + ABz * APz;

	// tt���v�Z
	double tt = ABdotAP / AB2;

	// �����̑�
	intersectionpoint.x = static_cast<float>(segment.startpoint.x + ABx * tt);
	intersectionpoint.y = static_cast<float>(segment.startpoint.y + ABy * tt);
	intersectionpoint.z = static_cast<float>(segment.startpoint.z + ABz * tt);

	t = static_cast<float>(tt);
	// �����̑��̒���
	distance = (intersectionpoint - point).Length();
	return distance;
}

// �����ƒ����̒��������߂�
float calcPointSegmentDist(
	const Vector3& p,
	const Segment& segment,
	Vector3& intersectionpoint,
	float& t) {
	float distance = calcPointLineDist(p, segment, intersectionpoint, t);

	// ��_�������̊O�ɂ���i�n�_�ɋ߂��j
	if (t < 0.0f) {
		intersectionpoint = segment.startpoint;		// �J�n�_����_

		float l = (p - intersectionpoint).Length();	// ��_�Ƃ̋��������߂�

		return l;
	}

	// ��_�������̊O�ɂ���i�I�_�ɋ߂��j
	if (t > 1.0f) {
		intersectionpoint = segment.endpoint;		// �I�_����_

		float l = (p - intersectionpoint).Length();	// ��_�Ƃ̋��������߂�

		return l;
	}

	// ��_��������ɂ���i�O�������P�j
	return distance;
}

// ���������������i�l�p�`�j�Ƌ��̓����蔻����s��
bool CollisionSphereOrientedQuad(
	Vector3 StartPos,						// �J�n�ʒu
	Vector3 EndPos,							// �I���ʒu
	float width,							// ��
	float radius,							// ���a
	Vector3 CirclePos)						// ���S���W
{
	Vector3 v[4];							// �l�p�`���_
	Vector3 intersectionpoint{};			// ��_

	// �����ɕ��s�ȂS�p�`�̍��W�����߂�
	CalcQuadOrientedLine(StartPos, EndPos, v, width);

	// �����i�S�p�`�̐����j
	Segment s[4];
	s[0].startpoint = v[0];
	s[0].endpoint = v[1];

	s[1].startpoint = v[2];
	s[1].endpoint = v[3];

	s[2].startpoint = v[0];
	s[2].endpoint = v[2];

	s[3].startpoint = v[1];
	s[3].endpoint = v[3];

	float t;

	// �l�p�`���͂ސ����Ɠ_�̋����𒲂ׂ�
	for (int idx = 0; idx < 4; idx++) {
		// �~�̒��S�Ɛ����̋��������߂�
		float distance = calcPointSegmentDist(CirclePos, s[idx], intersectionpoint, t);

		if (distance < radius) {
			return true;
		}
	}

	return false;
}

// �܂������ɗ����Ă�~���Ƌ��̓����蔻��
bool CollisionSphereCylinder(BoundingSphere sphere, BoundingCylinder cylinder)
{
	// XZ���ʂł̃`�F�b�N
	{
		// XZ���ʂł̒��S�ԋ��������߂�
		Vector3 p1center = sphere.center;
		Vector3 p2center = cylinder.bottom;

		p1center.y = 0.0f;
		p2center.y = 0.0f;

		float length = (p1center - p2center).Length();

		// �d�Ȃ肪���邩�H
		if (sphere.radius + cylinder.radius < length) {
			return false;
		}
	}

	// XY���ʂł̃`�F�b�N
	{
		// XY���ʂōl����
		Vector3 p1center = sphere.center;				// ���̒��S
		Vector3 p2bottomcenter = cylinder.bottom;		// �~���̒�
		Vector3 p2topcenter = cylinder.top;				// �~���̏��

		p1center.z = 0.0f;

		p2bottomcenter.z = 0.0f;
		p2topcenter.z = 0.0f;

		// ����
		Segment seg;
		seg.startpoint = p2bottomcenter;
		seg.endpoint = p2topcenter;

		bool sts = CollisionSphereOrientedQuad(
			seg.startpoint,							// �J�n�ʒu
			seg.endpoint,							// �I���ʒu
			cylinder.radius * 2.0f,					// ��
			sphere.radius,							// ���a
			p1center);								// ���S���W

		if (sts) {
			return true;
		}
	}

	// YZ���ʂł̃`�F�b�N
	{
		// YZ���ʂōl����
		Vector3 p1center = sphere.center;							// ���̒��S
		Vector3 p2bottomcenter = cylinder.bottom;					// �~���̒�
		Vector3 p2topcenter = cylinder.top;							// �~���̏��

		p1center.z = 0.0f;

		p2bottomcenter.x = 0.0f;
		p2topcenter.x = 0.0f;

		Segment seg;			// ����
		seg.startpoint = p2bottomcenter;
		seg.endpoint = p2topcenter;

		bool sts = CollisionSphereOrientedQuad(
			seg.startpoint,							// �J�n�ʒu
			seg.endpoint,							// �I���ʒu
			cylinder.radius * 2.0f,					// ��
			sphere.radius,							// ���a
			p1center);								// ���S���W

		if (sts) {
			return true;
		}
	}

	return false;
}

// ��
bool CollisionSphere(BoundingSphere p1, BoundingSphere p2)
{
	float length = (p1.center - p2.center).Length();

	if (p1.radius + p2.radius > length) {
		return false;
	}

	return true;
}

// AABB
bool CollisionAABB(AABB p1, AABB p2) {
	// X���W
	if (p1.max.x < p2.min.x) {
		return false;
	}

	if (p1.min.x > p2.max.x) {
		return false;
	}

	// Y���W
	if (p1.max.y < p2.min.y) {
		return false;
	}

	if (p1.min.y > p2.max.y) {
		return false;
	}

	// Z���W
	if (p1.max.z < p2.min.z) {
		return false;
	}

	if (p1.min.z > p2.max.z) {
		return false;
	}

	return true;
}

AABB SetAABB(Vector3 centerposition, float width, int height, int depth) {
	AABB aabb{};

	width = width;
	height = height;
	depth = depth;

	width = fabs(width);
	height = fabs(height);
	depth = fabs(depth);

	aabb.min.x = centerposition.x - width / 2.0f;
	aabb.min.y = centerposition.y - height / 2.0f;
	aabb.min.z = centerposition.z - depth / 2.0f;

	aabb.max.x = centerposition.x + width / 2.0f;
	aabb.max.y = centerposition.y + height / 2.0f;
	aabb.max.z = centerposition.z + depth / 2.0f;

	return aabb;
}

// �O�p�`�̓������ǂ����𔻒肷��
bool CheckInTriangle(
	const Vector3& a,
	const Vector3& b,
	const Vector3& c,
	const Vector3& p) {
	Vector3	ab, bc, ca;			// �R�ӂ̃x�N�g��
	Vector3	ap, bp, cp;			// �����̓_�Ƃ̃x�N�g��
	Vector3	normal;				// �R�p�`�̖@���x�N�g��
	Vector3	n1, n2, n3;			// �R�ӂƓ����̓_�Ƃ̖@���x�N�g��

	ab = b - a;

	bc = c - b;

	ca = a - c;

	ap = p - a;

	bp = p - b;

	cp = p - c;

	normal = ab.Cross(bc);

	n1 = ab.Cross(ap);

	n2 = bc.Cross(bp);

	n3 = ca.Cross(cp);

	float dot = n1.Dot(normal);
	if (dot < 0) return false;			// �ׂ��p�x���݊p

	dot = n2.Dot(normal);
	if (dot < 0) return false;			// �ׂ��p�x���݊p

	dot = n3.Dot(normal);
	if (dot < 0) return false;			// �ׂ��p�x���݊p

	return true;
}

bool LinetoPlaneCross(
	const Plane& plane,		//���ʂ̕�����
	const Vector3& p0,			//�����̋N�_
	const Vector3& wv,			//�����̕����x�N�g��
	float& t,			// ��_�ʒu���
	Vector3& ans)		//��_���W
{
	//	float t = 0;

	Vector3 normal;				// ���ʂ̖@���x�N�g���i���K���ς݁j
	normal.x = plane.x;
	normal.y = plane.y;
	normal.z = plane.z;

	float dot;			// ����

	dot = wv.Dot(normal);
	// ���s�`�F�b�N(���ς��v�Z����)
	if (fabsf(dot) < FLT_EPSILON) {
		//		MessageBox(nullptr, "���s�ł��B", "���s�ł��B", MB_OK);

		printf("���s�ł� \n");
		return false;
	}

	// �����ɂ������߂鎮������
	float dot2;			// ���q
	dot2 = p0.Dot(normal);

	t = -(dot2 + plane.w) / dot;

	ans.x = p0.x + wv.x * t;
	ans.y = p0.y + wv.y * t;
	ans.z = p0.z + wv.z * t;
	return true;
}