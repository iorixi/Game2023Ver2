#include	"CPlaneMesh.h"
#include	<iostream>

void CPlaneMesh::Init(
	int divx, int divy,
	int width, int height,
	DirectX::SimpleMath::Color color,
	DirectX::SimpleMath::Vector3 normal,
	bool xzflag) 
{
	// �T�C�Y�Z�b�g�i���ƍ����j�iXY���ʁj
	m_width = static_cast<float>(width);
	m_height = static_cast<float>(height);

	// ������
	m_divY = divy;
	m_divX = divx;

	// �@���x�N�g��
	m_normal = normal;

	// ���_�J���[
	m_color = color;

	if (xzflag == true) {
		// ���_�f�[�^����
		CreateVertexXZ();
	}
	else {
		// ���_�f�[�^����
		CreateVertex();
	}
	// �C���f�b�N�X�f�[�^����
	CreateIndex();

}

void CPlaneMesh::CreateVertex() {
	// ���_�f�[�^�N���A
	m_vertices.clear();

	for (unsigned int y = 0; y <= m_divY; y++) {
		VERTEX_3D	vtx{};

		for (unsigned int x = 0; x <= m_divX; x++) {

			// ���_���W�Z�b�g
			vtx.Position.x = -m_width / 2.0f + x * m_width / m_divX;
			vtx.Position.y = -m_height / 2.0f + y * m_height / m_divY;
			vtx.Position.z = 0.0f;

			// �@���x�N�g���Z�b�g
			vtx.Normal = m_normal;				// �@�����Z�b�g

			vtx.Diffuse = m_color;					// �J���[�l�Z�b�g

			// �J��Ԃ��ɑΉ��i�e�N�X�`���́j
			float texu = 1.0f * m_divX;
			float texv = 1.0f * m_divY;
			vtx.TexCoord.x = (texu * x / m_divX);
			vtx.TexCoord.y = (texv * y / m_divY);

			m_vertices.emplace_back(vtx);		// ���_�f�[�^�Z�b�g
		}
	}
}

void CPlaneMesh::CreateVertexXZ() {
	// ���_�f�[�^�N���A
	m_vertices.clear();

	for (unsigned int y = 0; y <= m_divY; y++) {
		VERTEX_3D	vtx{};

		for (unsigned int x = 0; x <= m_divX; x++) {

			// ���_���W�Z�b�g
			vtx.Position.x = -m_width / 2.0f + x * m_width / m_divX;
			vtx.Position.y = 0.0f;
			vtx.Position.z = -m_height / 2.0f + y * m_height / m_divY;

			// �@���x�N�g���Z�b�g
			vtx.Normal = m_normal;				// �@�����Z�b�g

			vtx.Diffuse = m_color;					// �J���[�l�Z�b�g

			// �J��Ԃ��ɑΉ��i�e�N�X�`���́j
			float texu = 1.0f * m_divX;
			float texv = 1.0f * m_divY;
			vtx.TexCoord.x = (texu * x / m_divX);
			vtx.TexCoord.y = (texv * y / m_divY);

			m_vertices.emplace_back(vtx);		// ���_�f�[�^�Z�b�g
		}
	}
}

void CPlaneMesh::CreateIndex() {
	// �C���f�b�N�X�f�[�^�N���A
	m_indices.clear();

	// �C���f�b�N�X����
	for (unsigned int y = 0; y < m_divY; y++) {
		for (unsigned int x = 0; x < m_divX; x++) {

			int count = (m_divX + 1) * y + x;		// �������W�̃C���f�b�N�X

			// ������
			{
				FACE face{};

				face.idx[0] = count;						// ����
				face.idx[1] = count + 1 + (m_divX + 1);		// �E��
				face.idx[2] = count + 1;					// �E

				m_indices.emplace_back(face.idx[0]);
				m_indices.emplace_back(face.idx[1]);
				m_indices.emplace_back(face.idx[2]);

			}

			// �㔼��
			{
				FACE face{};
				face.idx[0] = count;						// ����
				face.idx[1] = count + (m_divX + 1);			// ��
				face.idx[2] = count + (m_divX + 1) + 1;		// �E��

				m_indices.emplace_back(face.idx[0]);
				m_indices.emplace_back(face.idx[1]);
				m_indices.emplace_back(face.idx[2]);

			}
		}
	}
}

// ��
float CPlaneMesh::GetWidth() {
	return m_width;
}

// ����
float CPlaneMesh::GetHeight() {
	return m_height;
}

int CPlaneMesh::GetDivX() {
	return m_divX;
}

int CPlaneMesh::GetDivY() {
	return m_divY;
}

// �w�肵��3�p�`�ԍ��̎O�p�`�C���f�b�N�X���擾
CPlaneMesh::FACE CPlaneMesh::GetTriangle(int triangleno) {

	FACE face;
	face.idx[0] = m_indices[triangleno * 3];
	face.idx[1] = m_indices[triangleno * 3 + 1];
	face.idx[2] = m_indices[triangleno * 3 + 2];
	return face;

}

// ���Ԗڂ̎l�p�`����������
int CPlaneMesh::GetSquareNo(DirectX::SimpleMath::Vector3 pos)
{
	// �����b�V����XZ���ʂ��x�[�X�ō쐬���Ă���
	double x = pos.x;
	double y = pos.z;

	// ���ʂ̕�
	double planewidth = m_width;

	// ���ʂ̍���
	double planeheight = m_height;

	// �}�b�v�`�b�v�T�C�Y�i��Βl�Ōv�Z�j
	double mapchipwidth = fabs(planewidth / m_divX);
	double mapchipheight = fabs(planeheight / m_divY);

	// �������_�̑��΍��W�ɕϊ�
	double relativex = x + (planewidth / 2.0);
	double relativey = y + (planeheight / 2.0);

	// �����牽�Ԗڂ��H
	unsigned int mapchipx = static_cast<unsigned int>(relativex / mapchipwidth);

	// �����牽�Ԗڂ��H
	unsigned int mapchipy = static_cast<unsigned int>(relativey / mapchipheight);

	// �������O�ԖڂƂ����ꍇ�̏��Ԃ��v�Z
	int squareno;
	squareno = mapchipy * m_divX + mapchipx;

	if (squareno < 0) {
		squareno = 0;
	}
	else {
		if (squareno > m_divX * m_divY - 1) {
			squareno = m_divX * m_divY - 1;
		}
	}

	//
	std::cout << "mapchipx mapchipy: " << mapchipx << "," << mapchipy << "," << squareno << "(" << x << "," << y << ")" << std::endl;

	return squareno;
}




void CPlaneMeshWithoutCommon::Init(int divx, int divy,
	int width, int height,
	DirectX::SimpleMath::Color color,
	DirectX::SimpleMath::Vector3 normal) {

	// �T�C�Y�Z�b�g�i���ƍ����j�iXY���ʁj
	m_width = static_cast<float>(width);
	m_height = static_cast<float>(height);

	// ������
	m_divY = divy;
	m_divX = divx;

	// �@���x�N�g��
	m_normal = normal;

	// ���_�J���[
	m_color = color;

	// �C���f�b�N�X�ƒ��_�f�[�^�𐶐�
	CreateIndex();
}

void CPlaneMeshWithoutCommon::CreateIndex() {
	struct FACE {
		unsigned int idx[3];
	};

	// �C���f�b�N�X�f�[�^�N���A
	m_indices.clear();
	m_vertices.clear();

	unsigned int quadnum = 0;

	for (int y = 0; y < m_divY; y++) {
		for (int x = 0; x < m_divX; x++) {

			VERTEX_3D v;

			v.Position.x = -m_width / 2.0f + x * m_width / m_divX;
			v.Position.y = -m_height / 2.0f + y * m_height / m_divY;
			v.Position.z = 0.0f;
			v.TexCoord.x = 0.0f;
			v.TexCoord.y = 1.0f;

			v.Normal = m_normal;

			v.Diffuse = m_color;
			m_vertices.emplace_back(v);

			v.Position.x = -m_width / 2.0f + (x + 1) * m_width / m_divX;
			v.Position.y = -m_height / 2.0f + y * m_height / m_divY;
			v.Position.z = 0.0f;
			v.TexCoord.x = 1.0f;
			v.TexCoord.y = 1.0f;
			v.Normal = m_normal;
			v.Diffuse = m_color;
			m_vertices.emplace_back(v);

			v.Position.x = -m_width / 2.0f + x * m_width / m_divX;
			v.Position.y = -m_height / 2.0f + (y + 1) * m_height / m_divY;
			v.Position.z = 0.0f;
			v.TexCoord.x = 0.0f;
			v.TexCoord.y = 0.0f;
			v.Normal = m_normal;
			v.Diffuse = m_color;
			m_vertices.emplace_back(v);

			v.Position.x = -m_width / 2.0f + (x + 1) * m_width / m_divX;
			v.Position.y = -m_height / 2.0f + (y + 1) * m_height / m_divY;
			v.Position.z = 0.0f;
			v.TexCoord.x = 1.0f;
			v.TexCoord.y = 0.0f;
			v.Diffuse = m_color;
			v.Normal = m_normal;
			m_vertices.emplace_back(v);

			FACE f;

			// �C���f�b�N�X�f�[�^�쐬
			f.idx[2] = quadnum * 4;
			f.idx[1] = quadnum * 4 + 1;
			f.idx[0] = quadnum * 4 + 2;
			m_indices.emplace_back(f.idx[0]);
			m_indices.emplace_back(f.idx[1]);
			m_indices.emplace_back(f.idx[2]);

			f.idx[2] = quadnum * 4 + 1;
			f.idx[1] = quadnum * 4 + 3;
			f.idx[0] = quadnum * 4 + 2;
			m_indices.emplace_back(f.idx[0]);
			m_indices.emplace_back(f.idx[1]);
			m_indices.emplace_back(f.idx[2]);

			quadnum++;
		}
	}

	DirectX::SimpleMath::Vector3 p0p1;
	DirectX::SimpleMath::Vector3 p0p2;
	DirectX::SimpleMath::Vector3 n;

	p0p1 = m_vertices[1].Position - m_vertices[0].Position;
	p0p2 = m_vertices[2].Position - m_vertices[0].Position;

	// �O�ς��v�Z
	p0p1.Cross(p0p1, n);

	for (auto& v : m_vertices) {
		v.Normal = n;
	}
}

// ��
float CPlaneMeshWithoutCommon::GetWidth() {
	return m_width;
}

// ����
float CPlaneMeshWithoutCommon::GetHeight() {
	return m_height;
}

int CPlaneMeshWithoutCommon::GetDivX() {
	return m_divX;
}

int CPlaneMeshWithoutCommon::GetDivY() {
	return m_divY;
}
