#include <WICTextureLoader.h>
#include "renderer.h"
#include "field.h"
#include "shader.h"
#include "collision.h"

using namespace DirectX::SimpleMath;

void Field::Init()
{
	AddComponent<Shader>()->Load("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");

	// �����b�V������
	m_planemesh.Init(
		100, 100,					// ������
		640,						// �T�C�Y
		640,						// �T�C�Y
		Color(1, 1, 1, 1),			// ���_�J���[
		Vector3(0, 0, -1),			// �@���x�N�g��
		true);						// XZ����

	// �ʉ���t����i��l���z�Łj
//	m_planemesh.MakeUndulation(
//		0.0f,						// �ŏ�
//		3.0f);						// �ő�

	m_planemesh.MakeUndulationPerlinnoise(
		10.0f,			// �ő�̍���
		4,				// �I�N�^�[�u��
		0.28f);			// �p�[�V�X�e���X

	// ���ʂ̕������𐶐��i�S�ʁj
	MakeEquatation();

	// �����_���[������
	m_renderer.Init(m_planemesh);

	// �e�N�X�`���ǂݍ���
	DirectX::CreateWICTextureFromFile(
		Renderer::GetDevice(),
		L"asset/texture/field.jpg",
		nullptr,
		&m_Texture);

	assert(m_Texture);
}

void Field::Uninit()
{
	for (auto& com : m_Component) {
		com->Uninit();
		delete com;
	}

	m_Component.clear();

	m_Texture->Release();
}

void Field::Update()
{
}

void Field::Draw()
{
	GetComponent<Shader>()->Draw();

	// ���[���h�}�g���N�X�ݒ�
	Matrix world, scale, rot, trans;
	scale = Matrix::CreateScale(m_Scale.x);
	rot = Matrix::CreateFromYawPitchRoll(m_Rotation.y, m_Rotation.x, m_Rotation.z);
	trans = Matrix::CreateTranslation(m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	// �e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//�����_���ŕ`��
	m_renderer.Draw();
}

// ���݈ʒu�̍��������߂�
float  Field::GetFieldHeight(Vector3 pos) {
	float t;

	// �ʐ���
	for (unsigned int idx = 0; idx < m_planes.size(); idx++) {
		Vector3 up = { 0,1,0 };
		Vector3 startpoint = { pos.x,0,pos.z };
		Plane p = m_planes[idx].GetPlaneInfo().plane;
		Vector3 ans;

		bool sts = LinetoPlaneCross(p, startpoint, up, t, ans);
		if (sts) {
			sts = CheckInTriangle(
				m_planes[idx].GetPlaneInfo().p0,
				m_planes[idx].GetPlaneInfo().p1,
				m_planes[idx].GetPlaneInfo().p2, ans);
			if (sts) {
				return ans.y;
			}
		}
	}

	// ������Ȃ������ꍇ�͂O
	return 0;
}

// ���݈ʒu�̍��������߂�i�l�p�`�ԍ����w�肵�āj
float  Field::GetFieldHeightBySqno(Vector3 pos) {
	float t;

	// ���݈ʒu����̂����Ă���l�p�`�ԍ����擾
	int sqno = m_planemesh.GetSquareNo(pos);

	static float oldheight = 0;

	// ���ʃ`�F�b�N
	{
		int idx = sqno * 2;

		// �ʐ���
		Vector3 up = { 0,1,0 };
		Vector3 startpoint = { pos.x,0,pos.z };
		Plane p = m_planes[idx].GetPlaneInfo().plane;
		Vector3 ans;

		bool sts = LinetoPlaneCross(p, startpoint, up, t, ans);
		if (sts) {
			sts = CheckInTriangle(
				m_planes[idx].GetPlaneInfo().p0,
				m_planes[idx].GetPlaneInfo().p1,
				m_planes[idx].GetPlaneInfo().p2, ans);
			if (sts) {
				oldheight = ans.y;
				return ans.y;
			}
		}
	}

	// ��ʃ`�F�b�N
	{
		int idx = sqno * 2 + 1;

		// �ʐ���
		Vector3 up = { 0,1,0 };
		Vector3 startpoint = { pos.x,0,pos.z };
		Plane p = m_planes[idx].GetPlaneInfo().plane;
		Vector3 ans;

		bool sts = LinetoPlaneCross(p, startpoint, up, t, ans);
		if (sts) {
			sts = CheckInTriangle(
				m_planes[idx].GetPlaneInfo().p0,
				m_planes[idx].GetPlaneInfo().p1,
				m_planes[idx].GetPlaneInfo().p2, ans);
			if (sts) {
				oldheight = ans.y;
				return ans.y;
			}
		}
	}

	// ������Ȃ������ꍇ�͂O
	std::cout << "���Ȃ�" << "(" << sqno << ")" << std::endl;
	return oldheight;
}

// ���ʂ̕������𐶐�����
void Field::MakeEquatation() {
	// �ʐ����擾
	int numface;
	numface = m_planemesh.GetIndices().size() / 3;

	std::vector<VERTEX_3D> vertices;
	vertices = m_planemesh.GetVertices();

	// �ʐ������[�v
	for (unsigned int idx = 0; idx < numface; idx++) {
		CPlaneMesh::FACE f = m_planemesh.GetTriangle(idx);

		VERTEX_3D v0 = vertices[f.idx[0]];
		VERTEX_3D v1 = vertices[f.idx[1]];
		VERTEX_3D v2 = vertices[f.idx[2]];

		CPlane p;
		p.MakeEquatation(v0.Position, v1.Position, v2.Position);
		m_planes.emplace_back(p);
	}
}