#pragma once
#include	<vector>
#include	"../renderer.h"
#include	"CStaticMesh.h"
#include	"CShader.h"
#include	"../CMeshRenderer.h"
#include "../modelRenderer.h"

class CStaticMeshRenderer : public CMeshRenderer {
public:
	void Draw() override;
	void Init(CStaticMesh& mesh);
	void Uninit();
private:
	std::vector<ID3D11ShaderResourceView*>	m_DiffuseSRVtable;		// �f�B�t���[�Y�e�N�X�`��SRV
	std::vector<SUBSET>	m_Subsets;									// �T�u�Z�b�g���

	std::vector<CMaterial*>		m_Materials;						// �}�e���A���Q
};
