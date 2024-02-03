#pragma once
#include <SimpleMath.h>
#include <cmath>

// �x�N�g���̊p�x���v�Z����֐�
float CalculateAngle(const DirectX::SimpleMath::Vector3& v1, const DirectX::SimpleMath::Vector3& v2) {
	// ���ς��v�Z
	float dotProduct = v1.Dot(v2);

	// v1��v2�̒������擾
	float lengthV1 = v1.Length();
	float lengthV2 = v2.Length();

	// 0���Z�������
	if (lengthV1 == 0.0f || lengthV2 == 0.0f) {
		return 0.0f;
	}

	// ���ς̒l���p�x�ɕϊ����ĕԂ�
	return acos(dotProduct / (lengthV1 * lengthV2));
}