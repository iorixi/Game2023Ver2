#pragma once
#include	<SimpleMath.h>

// �n�`�f�[�^�̖ʂ̕��������
struct PLANEINFO {
	DirectX::SimpleMath::Plane		plane;			//���ʂ̕�����
	DirectX::SimpleMath::Vector3	p0, p1, p2;		//���_���W
};

// ���ʃN���X
class CPlane {
	PLANEINFO	m_planeinfo;
public:
	void MakeEquatation(
			DirectX::SimpleMath::Vector3 p0,
			DirectX::SimpleMath::Vector3 p1,
			DirectX::SimpleMath::Vector3 p2
	);
	PLANEINFO GetPlaneInfo() {
		return m_planeinfo;
	}
};

