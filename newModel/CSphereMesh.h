#pragma once
#include	<simplemath.h>
#include	"CMesh.h"

class CSphereMesh : public CMesh {
// �@���x�N�g�����v�Z
// ���_���W���m�[�}���C�Y
	void Normalize(DirectX::SimpleMath::Vector3 vec, DirectX::SimpleMath::Vector3& Normal) {
		vec.Normalize();
		Normal = vec;
	}

public:
	void Init(float radius,
		DirectX::SimpleMath::Color color,
		int division_horizontal, 
		int division_vertical) 
	{
		// ��������ۑ�
		m_divX = division_horizontal;
		m_divY = division_vertical;

		// ���a��ۑ�
		m_radius = radius;

		// �J���[�l��ۑ�
		m_color = color;

		// ���̃C���f�b�N�X�f�[�^���쐬
		CreateIndex();

		// ���̒��_�f�[�^���쐬
		CreateVertex();
	}

	void CreateVertex() {
		// ���_�f�[�^�N���A
		m_vertices.clear();

		float azimuth = 0.0f;			// ���ʊp
		float elevation = 0.0f;			// �p

		DirectX::SimpleMath::Vector3	Normal;

		// ���ʊp�Ƌp���狅���b�V���̒��_�f�[�^���쐬
		for (unsigned int y = 0; y <= m_divY; y++) {
			elevation = (DirectX::XM_PI * (float)y) / (float)m_divY;    // �p���Z�b�g
			float r = m_radius * sinf(elevation);						// �p�ɉ��������a���v�Z

			for (unsigned int x = 0; x <= m_divX; x++) {
				azimuth = (2 * DirectX::XM_PI * (float)x) / (float)m_divX;	// ���ʊp���Z�b�g

				VERTEX_3D v;
				v.Position.x = r * cosf(azimuth);
				v.Position.y = m_radius * cosf(elevation);
				v.Position.z = r * sinf(azimuth);

				Normalize(v.Position, Normal);		// �@�����v�Z
				v.Normal = Normal;					// �@�����Z�b�g

				v.Diffuse = m_color;				// ���_�J���[

				m_vertices.emplace_back(v);
			}
		}
	}

	void CreateIndex() {
		struct FACE {
			unsigned int idx[3];
		};

		// �C���f�b�N�X�f�[�^�N���A
		m_indices.clear();

		// �C���f�b�N�X����
		for (unsigned int y = 0; y < m_divY; y++) {
			for (unsigned int x = 0; x < m_divX; x++) {
				int count = (m_divX + 1) * y + x;			// ������W�̃C���f�b�N�X

				FACE f;
				// �㔼��
				f.idx[0] = count;
				f.idx[1] = count + 1;
				f.idx[2] = count + 1 + (m_divX + 1);

				m_indices.emplace_back(f.idx[0]);
				m_indices.emplace_back(f.idx[1]);
				m_indices.emplace_back(f.idx[2]);

				// ������
				f.idx[0] = count;
				f.idx[1] = count + (m_divX + 1) + 1;
				f.idx[2] = count + (m_divX + 1);

				m_indices.emplace_back(f.idx[0]);
				m_indices.emplace_back(f.idx[1]);
				m_indices.emplace_back(f.idx[2]);
			}
		}
	}
private:
	unsigned int m_divX = 1;
	unsigned int m_divY = 1;
	float m_radius = 100.0f;
	DirectX::SimpleMath::Color m_color;
};


