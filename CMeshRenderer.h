#pragma once
#include	"CVertexBuffer.h"
#include	"CIndexBuffer.h"
#include	"CMesh.h"

class CMeshRenderer {
protected:
	CVertexBuffer<VERTEX_3D>	m_VertexBuffer;		// 頂点バッファ
	CIndexBuffer				m_IndexBuffer;		// インデックスバッファ
	int							m_IndexNum = 0;		// インデックス数
public:
	virtual void Init(CMesh& mesh);
	virtual void Draw();
};
