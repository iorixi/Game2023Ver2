#include	"CAnimMeshRenderer.h"
#include	"CAnimMesh.h"

void CAnimMeshRenderer::Init(CAnimMesh& mesh) {
	// 頂点バッファ生成
	m_VertexBuffer.Create(mesh.GetVertices());

	// インデックスバッファ生成
	m_IndexBuffer.Create(mesh.GetIndices());

	// マテリアル情報を取得しマテリアル毎に定数バッファを生成する
	const std::vector<MATERIAL>& materials = mesh.GetMaterials();

	for (auto& mtrl : materials) {
		CMaterial* m = new CMaterial();
		m->Init(mtrl);
		m_Materials.emplace_back(m);
	}

	// ディフューズテクスチャテーブルをメッシュから取得
	m_DiffuseSRVtable = mesh.GetSRVtable();

	// SUBSET情報取得
	m_Subsets = mesh.GetSubsets();

	// 定数バッファ生成（ボーンコンビネーション格納用）
	bool sts = CreateConstantBufferWrite(											// 20231227
		Renderer::GetDevice(),					    // デバイスオブジェクト			// 20231227
		sizeof(CBBoneCombMatrix),					// コンスタントバッファサイズ	// 20231227
		&m_BoneCombMtxCBuffer);						// コンスタントバッファ			// 20231227

	assert(m_BoneCombMtxCBuffer);
}

void CAnimMeshRenderer::Uninit() {
	for (auto& mtrl : m_Materials) {
		mtrl->Uninit();
		delete mtrl;
	}
}

void CAnimMeshRenderer::Draw() {
	// デバイスコンテキスト取得
	ID3D11DeviceContext* devicecontext;
	devicecontext = Renderer::GetDeviceContext();

	// 頂点バッファ設定
	m_VertexBuffer.SetGPU();
	// インデックスバッファ設定
	m_IndexBuffer.SetGPU();

	// トポロジーをセット（旧プリミティブタイプ）
	devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 20231227 レジスタ５にセット
	devicecontext->VSSetConstantBuffers(5, 1, &m_BoneCombMtxCBuffer);

	// サブセットの数分ループ（サブセット数はマテリアルの数）
	for (auto& subset : m_Subsets) {
		// SRVをセット
		devicecontext->PSSetShaderResources(0, 1, &m_DiffuseSRVtable[subset.MaterialIdx]);

		// マテリアル定数バッファをGPUにセット
		m_Materials[subset.MaterialIdx]->SetGPU();

		// ドローコール
		devicecontext->DrawIndexed(
			subset.IndexNum,	// 描画するインデックス数
			subset.IndexBase,	// インデックスバッファベース開始位置（この場所がインデックス０番目になる）
			subset.VertexBase);	// 頂点バッファベース開始位置（この場所が頂点の０番目になる）
	}
}

// ボーンコンビネーション行列格納用
void CAnimMeshRenderer::Update(CAnimMesh& mesh)
{
	// メッシュから更新されたボーンコンビネーション行列群を取得
	const std::vector<aiMatrix4x4> mtxcontainer = mesh.GetBoneCombinationMatrix();

	// 定数バッファ更新
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	CBBoneCombMatrix* pData = nullptr;

	HRESULT hr = Renderer::GetDeviceContext()->Map(
		m_BoneCombMtxCBuffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&MappedResource);

	if (SUCCEEDED(hr)) {
		pData = (CBBoneCombMatrix*)MappedResource.pData;
		memcpy(pData->BoneCombMtx,
			mtxcontainer.data(),
			sizeof(aiMatrix4x4) * mtxcontainer.size());
		Renderer::GetDeviceContext()->Unmap(m_BoneCombMtxCBuffer, 0);
	}
}