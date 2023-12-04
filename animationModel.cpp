#include <SimpleMath.h>
#include "main.h"
#include "renderer.h"
#include "animationModel.h"
#include "utility.h"

using namespace DirectX::SimpleMath;

void AnimationModel::Draw()
{
	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = Color(0.2f, 0.2f, 0.2f, 1.0f);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	// 20230909-02 レジスタ５にセット
	Renderer::GetDeviceContext()->VSSetConstantBuffers(5, 1, &m_BoneCombMtxCBuffer);

	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_AiScene->mMeshes[m];

		aiMaterial* material = m_AiScene->mMaterials[mesh->mMaterialIndex];

		// テクスチャ設定
		aiString path;
		material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1,
			&m_Texture[path.data]);

		// 頂点バッファ設定
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1,
			&m_VertexBuffer[m], &stride, &offset);

		// インデックスバッファ設定
		Renderer::GetDeviceContext()->IASetIndexBuffer(
			m_IndexBuffer[m], DXGI_FORMAT_R32_UINT, 0);

		// ポリゴン描画
		Renderer::GetDeviceContext()->DrawIndexed(
			mesh->mNumFaces * 3, 0, 0);
	}
}

// 頂点バッファ生成
void AnimationModel::CreateVertexBufferPerMesh(int m, const aiMesh* mesh) {
	VERTEX_3D* vertex = new VERTEX_3D[mesh->mNumVertices];

	//変形後頂点データ初期化
	for (unsigned int v = 0; v < mesh->mNumVertices; v++)
	{
		DEFORM_VERTEX deformVertex;
		deformVertex.Position = mesh->mVertices[v];
		deformVertex.Normal = mesh->mNormals[v];
		deformVertex.BoneNum = 0;

		for (unsigned int b = 0; b < 4; b++)
		{
			deformVertex.BoneName[b] = "";
			deformVertex.BoneWeight[b] = 0.0f;
		}

		m_DeformVertex[m].push_back(deformVertex);
	}

	//ボーンデータ生成
	for (unsigned int b = 0; b < mesh->mNumBones; b++)
	{
		aiBone* bone = mesh->mBones[b];

		m_Bone[bone->mName.C_Str()].OffsetMatrix = bone->mOffsetMatrix;

		//変形後頂点にボーンデータ格納
		for (unsigned int w = 0; w < bone->mNumWeights; w++)
		{
			aiVertexWeight weight = bone->mWeights[w];

			int num = m_DeformVertex[m][weight.mVertexId].BoneNum;

			m_DeformVertex[m][weight.mVertexId].BoneWeight[num] = weight.mWeight;
			m_DeformVertex[m][weight.mVertexId].BoneName[num] = bone->mName.C_Str();

			// 20230909
			m_DeformVertex[m][weight.mVertexId].BoneIdx[num] = m_Bone[bone->mName.C_Str()].idx;

			m_DeformVertex[m][weight.mVertexId].BoneNum++;

			assert(m_DeformVertex[m][weight.mVertexId].BoneNum <= 4);
		}
	}

	// 頂点データ生成
	for (unsigned int v = 0; v < mesh->mNumVertices; v++)
	{
		vertex[v].Position = Vector3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z);
		vertex[v].Normal = Vector3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);
		if (mesh->mTextureCoords[0] == nullptr)
		{
			vertex[v].TexCoord = Vector2(0, 0);
		}
		else
		{
			vertex[v].TexCoord = Vector2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);
		}
		vertex[v].Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);

		vertex[v].BoneIndex[0] = m_DeformVertex[m][v].BoneIdx[0];
		vertex[v].BoneIndex[1] = m_DeformVertex[m][v].BoneIdx[1];
		vertex[v].BoneIndex[2] = m_DeformVertex[m][v].BoneIdx[2];
		vertex[v].BoneIndex[3] = m_DeformVertex[m][v].BoneIdx[3];

		vertex[v].BoneWeight[0] = m_DeformVertex[m][v].BoneWeight[0];
		vertex[v].BoneWeight[1] = m_DeformVertex[m][v].BoneWeight[1];
		vertex[v].BoneWeight[2] = m_DeformVertex[m][v].BoneWeight[2];
		vertex[v].BoneWeight[3] = m_DeformVertex[m][v].BoneWeight[3];
	}

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * mesh->mNumVertices;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(
		&bd,
		&sd,
		&m_VertexBuffer[m]);

	delete[] vertex;
}

// インデックスバッファを生成
void AnimationModel::CreateIndexBufferPerMesh(int m, const aiMesh* mesh) {
	unsigned int* index = new unsigned int[mesh->mNumFaces * 3];

	for (unsigned int f = 0; f < mesh->mNumFaces; f++)
	{
		const aiFace* face = &mesh->mFaces[f];

		assert(face->mNumIndices == 3);

		index[f * 3 + 0] = face->mIndices[0];
		index[f * 3 + 1] = face->mIndices[1];
		index[f * 3 + 2] = face->mIndices[2];
	}

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(unsigned int) * mesh->mNumFaces * 3;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = index;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer[m]);

	delete[] index;
}

void AnimationModel::Load(const char* FileName)
{
	const std::string modelPath(FileName);

	m_AiScene = aiImportFile(FileName, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded);
	assert(m_AiScene);

	m_VertexBuffer = new ID3D11Buffer * [m_AiScene->mNumMeshes];
	m_IndexBuffer = new ID3D11Buffer * [m_AiScene->mNumMeshes];

	//変形後頂点配列生成
	m_DeformVertex = new std::vector<DEFORM_VERTEX>[m_AiScene->mNumMeshes];

	//再帰的にボーンの階層構造を生成（ボーン名で検索できる構造を作成）
	CreateBone(m_AiScene->mRootNode);

	// ボーンの配列位置を格納する				// 20230909
	unsigned int num = 0;						// 20230909
	for (auto& data : m_Bone) {					// 20230909
		data.second.idx = num;					// 20230909
		num++;									// 20230909
	}											// 20230909

	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		const aiMesh* mesh = m_AiScene->mMeshes[m];

		// インデックスバッファを生成
		CreateIndexBufferPerMesh(m, mesh);

		// 頂点バッファを生成
		CreateVertexBufferPerMesh(m, mesh);
	}

	//テクスチャ読み込み
	for (int i = 0; i < m_AiScene->mNumTextures; i++)
	{
		ID3D11ShaderResourceView* texture;

		aiTexture* aitexture = m_AiScene->mTextures[i];

		// mHeight is 0, so try to load a compressed texture of mWidth bytes
		const size_t size = aitexture->mWidth;

		HRESULT hr = DirectX::CreateWICTextureFromMemory(
			Renderer::GetDevice(),
			Renderer::GetDeviceContext(),
			reinterpret_cast<const unsigned char*>(aitexture->pcData),
			aitexture->mWidth,
			nullptr,
			&texture);

		assert(texture);

		m_Texture[aitexture->mFilename.data] = texture;
	}

	// 定数バッファ生成　20230909-02
	bool sts = CreateConstantBufferWrite(											// 20230909 - 02
		Renderer::GetDevice(),					    // デバイスオブジェクト			// 20230909 - 02
		sizeof(CBBoneCombMatrx),					// コンスタントバッファサイズ	// 20230909 - 02
		&m_BoneCombMtxCBuffer);						// コンスタントバッファ			// 20230909 - 02

	assert(m_BoneCombMtxCBuffer);
}

void AnimationModel::LoadAnimation(const char* FileName, const char* Name)
{
	m_Animation[Name] = aiImportFile(FileName, aiProcess_ConvertToLeftHanded);
	assert(m_Animation[Name]);
}

void AnimationModel::CreateBone(aiNode* node)
{
	BONE bone;

	m_Bone[node->mName.C_Str()] = bone;

	for (unsigned int n = 0; n < node->mNumChildren; n++)
	{
		CreateBone(node->mChildren[n]);
	}
}

void AnimationModel::Uninit()
{
	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		m_VertexBuffer[m]->Release();
		m_IndexBuffer[m]->Release();
	}

	delete[] m_VertexBuffer;
	delete[] m_IndexBuffer;

	delete[] m_DeformVertex;

	for (std::pair<const std::string, ID3D11ShaderResourceView*> pair : m_Texture)
	{
		pair.second->Release();
	}

	m_BoneCombMtxCBuffer->Release();						// 20230909-02

	aiReleaseImport(m_AiScene);

	for (std::pair<const std::string, const aiScene*> pair : m_Animation)
	{
		aiReleaseImport(pair.second);
	}
}

void AnimationModel::Update(const char* AnimationName1, int Frame1, const char* AnimationName2, int Frame2, float BlendRate)
{
	// アニメーションありか？
	if (m_Animation.count(AnimationName1) == 0)
		return;
	if (m_Animation.count(AnimationName2) == 0)
		return;

	if (!m_Animation[AnimationName1]->HasAnimations())
		return;
	if (!m_Animation[AnimationName2]->HasAnimations())
		return;

	//アニメーションデータからボーンマトリクス算出
	aiAnimation* animation1 = m_Animation[AnimationName1]->mAnimations[0];
	aiAnimation* animation2 = m_Animation[AnimationName2]->mAnimations[0];

	for (unsigned int c = 0; c < animation1->mNumChannels; c++)
	{
		aiNodeAnim* nodeAnim = animation1->mChannels[c];

		BONE* bone = &m_Bone[nodeAnim->mNodeName.C_Str()];

		int f;

		f = Frame1 % nodeAnim->mNumRotationKeys;				//簡易実装
		aiQuaternion rot = nodeAnim->mRotationKeys[f].mValue;

		f = Frame1 % nodeAnim->mNumPositionKeys;				//簡易実装
		aiVector3D pos = nodeAnim->mPositionKeys[f].mValue;

		bone->AnimationMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), rot, pos);
	}

	//再帰的にボーンマトリクスを更新
//	aiMatrix4x4 rootMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), aiQuaternion(AI_MATH_PI, 0.0f, 0.0f), aiVector3D(0.0f, 0.0f, 0.0f));
	aiMatrix4x4 rootMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), aiQuaternion(AI_MATH_PI, 0.0f, 0.0f), aiVector3D(0.0f, 0.0f, 0.0f));

	UpdateBoneMatrix(m_AiScene->mRootNode, rootMatrix);

	// 20230909 ボーンコンビネーション行列の配列を生成する
	std::vector<aiMatrix4x4> bonecombmtxcontainer;					// 20230909
	bonecombmtxcontainer.resize(m_Bone.size());						// 20230909
	for (auto data : m_Bone) {										// 20230909
		bonecombmtxcontainer[data.second.idx] = data.second.Matrix;	// 20230909
	}																// 20230909

	// 20230909 転置
	for (auto& bcmtx : bonecombmtxcontainer)
	{
		// 転置する
		bcmtx.Transpose();
	}

	// 20230909-02 定数バッファに反映させる
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	CBBoneCombMatrx* pData = nullptr;

	HRESULT hr = Renderer::GetDeviceContext()->Map(
		m_BoneCombMtxCBuffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&MappedResource);

	if (SUCCEEDED(hr)) {
		pData = (CBBoneCombMatrx*)MappedResource.pData;
		memcpy(pData->BoneCombMtx,
			bonecombmtxcontainer.data(),
			sizeof(aiMatrix4x4) * bonecombmtxcontainer.size());
		Renderer::GetDeviceContext()->Unmap(m_BoneCombMtxCBuffer, 0);
	}
}

void AnimationModel::UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix)
{
	// 引数で渡されたノード名をキーとしてボーン情報を取得する
	BONE* bone = &m_Bone[node->mName.C_Str()];

	//マトリクスの乗算順番に注意
	aiMatrix4x4 worldMatrix;					// 初期値は単位行列

	//	worldMatrix *= matrix;

	worldMatrix = matrix;						// 親の位置、姿勢が初期状態（逆ボーンオフセット行列）
	worldMatrix *= bone->AnimationMatrix;		// 引数で渡された行列を掛け算（自ノードのアニメーションを反映させる）（ボーン空間でのもの）

	bone->Matrix = worldMatrix;					// プログラム内に用意している行列に反映させる
	bone->Matrix *= bone->OffsetMatrix;			// オフセット行列を反映させる

	for (unsigned int n = 0; n < node->mNumChildren; n++)
	{
		UpdateBoneMatrix(node->mChildren[n], worldMatrix);
	}
}