#pragma once
#include	<simplemath.h>
#include	"../CMesh.h"

class CBoxMesh : public CMesh {
	// �@���x�N�g�����v�Z
	// ���_���W���m�[�}���C�Y
	void Normalize(
		DirectX::SimpleMath::Vector3 vec,
		DirectX::SimpleMath::Vector3& Normal);

public:
	void Init(float width,
		float height,
		float depth,
		DirectX::SimpleMath::Color color);

	// ���_�f�[�^����
	void CreateVertex();
	// �C���f�b�N�X����
	void CreateIndex();
private:
	DirectX::SimpleMath::Color m_color;
	float m_lengthx = 0.0f;
	float m_lengthy = 0.0f;
	float m_lengthz = 0.0f;
};
