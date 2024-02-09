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
	virtual void Init(CMesh& mesh);
	virtual void Draw();
};
