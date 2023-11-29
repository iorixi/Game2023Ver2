#pragma once
#include	"CVertexBuffer.h"
#include	"CIndexBuffer.h"
#include	"CMesh.h"

class CMeshRenderer {
protected:
	CVertexBuffer<VERTEX_3D>	m_VertexBuffer;		// ���_�o�b�t�@
	CIndexBuffer				m_IndexBuffer;		// �C���f�b�N�X�o�b�t�@
	int							m_IndexNum = 0;		// �C���f�b�N�X��
public:
	virtual void Init(CMesh& mesh) {
		m_VertexBuffer.Create(mesh.GetVertices());
		m_IndexBuffer.Create(mesh.GetIndices());
		m_IndexNum = static_cast<int>(mesh.GetIndices().size());
	}

	virtual void Draw() {
		ID3D11DeviceContext* devicecontext;

		devicecontext = Renderer::GetDeviceContext();

		// �g�|���W�[���Z�b�g�i���v���~�e�B�u�^�C�v�j
		devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		m_VertexBuffer.SetGPU();
		m_IndexBuffer.SetGPU();

//		Renderer::SetWire();

		devicecontext->DrawIndexed(
			m_IndexNum,					// �`�悷��C���f�b�N�X���i�ʐ��~�R�j
			0,							// �ŏ��̃C���f�b�N�X�o�b�t�@�̈ʒu
			0);							// ���_�o�b�t�@�̍ŏ�����g��

//		Renderer::SetSolid();
	}
};
