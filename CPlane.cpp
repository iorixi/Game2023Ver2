#include	<SimpleMath.h>
#include	"CPlane.h"

// ���ʂ̕��������쐬����
void CPlane::MakeEquatation(
	DirectX::SimpleMath::Vector3 p0,
	DirectX::SimpleMath::Vector3 p1,
	DirectX::SimpleMath::Vector3 p2)
{
	DirectX::SimpleMath::Plane p;
	DirectX::SimpleMath::Vector3		p0p1;
	DirectX::SimpleMath::Vector3		p1p2;
	DirectX::SimpleMath::Vector3		normal;

	p0p1 = p1 - p0;
	p1p2 = p2 - p1;

	// �O�ς����߂�
	normal = p0p1.Cross(p1p2);

	// ���K��
	normal.Normalize();

	// a,b,c,d�����߂�
	p.x = normal.x;
	p.y = normal.y;
	p.z = normal.z;
	p.w = -(p.x * p0.x + p.y * p0.y + p.z * p0.z);

	// �����o�ϐ��փZ�b�g
	m_planeinfo.plane = p;

	m_planeinfo.p0 = p0;
	m_planeinfo.p1 = p1;
	m_planeinfo.p2 = p2;
}