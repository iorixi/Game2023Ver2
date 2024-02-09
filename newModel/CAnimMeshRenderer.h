#pragma once
#include	<vector>
#include	<wrl/client.h>
#include	"../renderer.h"
#include	"../CMeshRenderer.h"
#include	"CAnimMesh.h"

// COMPTRを使えるように
using Microsoft::WRL::ComPtr;

// ボーンコンビネーション行列用の定数バッファ 20231227
struct CBBoneCombMatrix {
	DirectX::XMFLOAT4X4 BoneCombMtx[400];	// ボーンコンビネーション行列
};

class CAnimMeshRenderer : public CMeshRenderer {
public:
	void Draw() override;
	void Init(CAnimMesh& mesh);
	void Update(CAnimMesh& mesh);
	void Uninit();
private:
	ID3D11Buffer* m_BoneCombMtxCBuffer;						// 20231227

	std::vector<ID3D11ShaderResourceView*>	m_DiffuseSRVtable;		// ディフューズテクスチャSRV
	std::vector<SUBSET>	m_Subsets;									// サブセット情報

	std::vector<CMaterial*>		m_Materials;						// マテリアル群
};
