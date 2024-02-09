#pragma once
#include	<vector>
#include	<wrl/client.h>
#include	"../renderer.h"
#include	"../CMeshRenderer.h"
#include	"CAnimMesh.h"

// COMPTR���g����悤��
using Microsoft::WRL::ComPtr;

// �{�[���R���r�l�[�V�����s��p�̒萔�o�b�t�@ 20231227
struct CBBoneCombMatrix {
	DirectX::XMFLOAT4X4 BoneCombMtx[400];	// �{�[���R���r�l�[�V�����s��
};

class CAnimMeshRenderer : public CMeshRenderer {
public:
	void Draw() override;
	void Init(CAnimMesh& mesh);
	void Update(CAnimMesh& mesh);
	void Uninit();
private:
	ID3D11Buffer* m_BoneCombMtxCBuffer;						// 20231227

	std::vector<ID3D11ShaderResourceView*>	m_DiffuseSRVtable;		// �f�B�t���[�Y�e�N�X�`��SRV
	std::vector<SUBSET>	m_Subsets;									// �T�u�Z�b�g���

	std::vector<CMaterial*>		m_Materials;						// �}�e���A���Q
};
