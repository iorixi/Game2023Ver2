#include	<iostream>
#include	"CStaticMesh.h"
#include	"../renderer.h"

using namespace DirectX;

// モデルファイルを初期化する
bool CStaticMesh::Init(
	std::string filename)			//	３Ｄモデルファイル
{
	bool sts{};

	m_filename = filename;			// テクスチャ読み込み用に保存

	// シーン情報構築
	static Assimp::Importer importer;

	// シーン情報を構築
	m_AiScene = importer.ReadFile(
		filename.c_str(),
		//			aiProcess_ConvertToLeftHanded | aiProcess_Triangulate);
		aiProcessPreset_TargetRealtime_MaxQuality |
		aiProcess_ConvertToLeftHanded |
		aiProcess_PopulateArmatureData);		// 20231225追加

	//			aiProcess_ConvertToLeftHanded |		// 左手座標系に変換する
	//		aiProcess_PopulateArmatureData |
	//		aiProcess_PreTransformVertices |
			// ノード構造を削除して　それらのノードが持つローカル変換行列ですべての頂点を事前に変換する
	//			aiProcess_Triangulate);
			// 三角形化する
	//		aiProcess_GenSmoothNormals |
	//		aiProcess_JoinIdenticalVertices);

	if (m_AiScene == nullptr) {
		printf("load error %s ( %s ) \n ", filename.c_str(), importer.GetErrorString());
		return false;
	}

	//再帰的にボーン辞書（空で階層構造生成） 20231226追加
	CreateBone(m_AiScene->mRootNode);

	// ボーンの配列位置を格納する				// 20231226
	unsigned int num = 0;						// 20231226
	for (auto& data : m_Bone) {					// 20231226
		data.second.idx = num;					// 20231226
		num++;									// 20231226
	}											// 20231226

	// メッシュ数でメッシュコンテナ配列数を決定
	m_subsets.resize(m_AiScene->mNumMeshes);

	// メッシュ数分ループ
	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++) {
		aiMesh* mesh{};
		mesh = m_AiScene->mMeshes[m];

		// メッシュ単位で頂点データを取得
		int vnum = GetVerticesFromaiMesh(mesh);

		// メッシュ単位でインデックスデータ取得
		int idxnum = GetIndicesFromaiMesh(mesh);

		// メッシュで使用しているマテリアル情報を取得
		int mtrlidx = mesh->mMaterialIndex;

		// サブセット名をセットする
		m_subsets[m].Name = std::string(mesh->mName.C_Str());		// 20231225追加

		// インデックス数を保存
		m_subsets[m].IndexNum = idxnum;

		// 頂点数を保存
		m_subsets[m].VertexNum = vnum;

		// マテリアルインデックスを保存
		m_subsets[m].MaterialIdx = mtrlidx;

		// ボーン情報を取得する(サブセット単位で)
//			std::vector<C_BONE> subsetbone = GetBoneInfo(mesh);  for debug
		m_SubsetBones.emplace_back(GetBoneInfo(mesh));
	}

	// サブセットを１つの頂点バッファ・インデックスバッファ
	// に統合しているのでベース情報（スタート位置）
	// を計算で求めている
	CalcMeshBaseIndex();

	// ボーンIDとウエイト値を頂点データに反映させる
	// 必ずCalcMeshBaseIndex()の後で行う事
	SetBoneDataToVertices();

	// 全テクスチャを取得
	GetTextures(m_AiScene);

	// マテリアルを取得
	GetMaterials(m_AiScene);

	/*
	{
		FILE* fp;
		int cnt = 0;
		fopen_s(&fp, "vertex.txt", "w");
		for (auto& v : m_vertices) {
			fprintf(fp, "(%d) pos %f %f %f bonename %s %s %s %s  boneindex %d %d %d %d boneweight %f %f %f %f\n",
				cnt,
				v.Position.x, v.Position.y, v.Position.z,
//					v.BoneName[0].c_str(), v.BoneName[1].c_str(), v.BoneName[2].c_str(), v.BoneName[3].c_str(),
					v.BoneIndex[0], v.BoneIndex[1], v.BoneIndex[2], v.BoneIndex[3],
					v.BoneWeight[0], v.BoneWeight[1], v.BoneWeight[2], v.BoneWeight[3]);
				cnt++;
			}
			fclose(fp);
		}

		*/

		/*
		{
			FILE* fp;
			int i = 0;
			fopen_s(&fp, "index.txt", "w");
			for (auto& index : m_indices) {
				fprintf(fp, "(%d) %d \n", i, index);
				i++;
			}
			fclose(fp);
		}
		*/

	return true;
}

void CStaticMesh::Exit() {
	// SRV解放
//	for (auto& srv : m_AllTexture) {
//		srv->Release();
//	}

	// SRV解放
	for (auto& texres : m_Texturedic) {
		if (texres.second != nullptr) {
			texres.second->Release();
		}
	}
}

// 全頂点をaiMeshから取得する
unsigned int  CStaticMesh::GetVerticesFromaiMesh(const aiMesh* aimesh) {
	// 頂点数分ループ
	for (unsigned int vidx = 0; vidx < aimesh->mNumVertices; vidx++) {
		VERTEX_3D v;

		v.Position.x = aimesh->mVertices[vidx].x;
		v.Position.y = aimesh->mVertices[vidx].y;
		v.Position.z = aimesh->mVertices[vidx].z;

		// 法線あり？
		if (aimesh->HasNormals()) {
			v.Normal.x = aimesh->mNormals[vidx].x;
			v.Normal.y = aimesh->mNormals[vidx].y;
			v.Normal.z = aimesh->mNormals[vidx].z;
		}
		else {
			v.Normal = XMFLOAT3(0, 0, 0);
		}

		// テクスチャあり？
		if (aimesh->HasTextureCoords(0)) {
			v.TexCoord.x = aimesh->mTextureCoords[0][vidx].x;				// 20230809 bug fix by ts
			v.TexCoord.y = aimesh->mTextureCoords[0][vidx].y;				// 20230809 bug fix by ts
		}
		else {
			v.TexCoord = XMFLOAT2(0, 0);
		}

		// 頂点カラーあり？
		if (aimesh->HasVertexColors(0)) {
			v.Diffuse.x = aimesh->mColors[0][vidx].r;
			v.Diffuse.y = aimesh->mColors[0][vidx].g;
			v.Diffuse.z = aimesh->mColors[0][vidx].b;
			v.Diffuse.w = aimesh->mColors[0][vidx].a;
		}
		else {
			v.Diffuse = XMFLOAT4(1, 1, 1, 1);		// 無かった場合は白
		}

		// 頂点データを保存
		m_vertices.emplace_back(v);
	}

	// aiMeshが保持する頂点数を戻す
	return aimesh->mNumVertices;
}

// aiMeshから全インデックス取得
unsigned int  CStaticMesh::GetIndicesFromaiMesh(const aiMesh* aimesh) {
	// 面数分ループ
	for (unsigned int f = 0; f < aimesh->mNumFaces; f++)
	{
		const aiFace* face = &aimesh->mFaces[f];
		assert(face->mNumIndices <= 3);

		// インデックスデータを保存
		m_indices.emplace_back(face->mIndices[0]);
		m_indices.emplace_back(face->mIndices[1]);
		m_indices.emplace_back(face->mIndices[2]);
	}

	return aimesh->mNumFaces * 3;
}

// ベース情報を再計算
void CStaticMesh::CalcMeshBaseIndex() {
	// サブセット数分ループ
	for (int m = 0; m < m_subsets.size(); m++)
	{
		// 頂点バッファのベースを計算
		m_subsets[m].VertexBase = 0;
		for (int i = m - 1; i >= 0; i--) {
			m_subsets[m].VertexBase += m_subsets[i].VertexNum;
		}

		// インデックスバッファのベースを計算
		m_subsets[m].IndexBase = 0;
		for (int i = m - 1; i >= 0; i--) {
			m_subsets[m].IndexBase += m_subsets[i].IndexNum;
		}
	}
}

// すべてのテクスチャSRVを取得
void CStaticMesh::GetTextures(const aiScene* aiscene) {
	ID3D11DeviceContext* devicecontext;
	ID3D11Device* device;

	//		device = CDirectXGraphics::GetInstance()->GetDXDevice();
	//		devicecontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	device = Renderer::GetDevice();
	devicecontext = Renderer::GetDeviceContext();

	// このシーンが持つテクスチャを全て読み込む
	for (unsigned int texidx = 0; texidx < aiscene->mNumTextures; texidx++) {
		// テクスチャを取得
		aiTexture* aitexture = aiscene->mTextures[texidx];

		// 内蔵テクスチャか？
		if (isEmbeddedTexure(aitexture)) {
			ID3D11ShaderResourceView* srv;

			aiTexture* aitexture = aiscene->mTextures[texidx];

			// mHeight is 0, so try to load a compressed texture of mWidth bytes
			const size_t size = aitexture->mWidth;

			// SRV生成
			HRESULT hr = DirectX::CreateWICTextureFromMemory(
				device,
				devicecontext,
				reinterpret_cast<const unsigned char*>(aitexture->pcData),
				aitexture->mWidth,
				nullptr,
				&srv);

			assert(srv);

			// assimpの内部データにセットされているテクスチャ名をキーとして保存
			m_Texturedic[aitexture->mFilename.data] = srv;
		}
		else {
			// 内蔵テクスチャではない場合

			// テクスチャファイル名を取得する
			std::string filename = GetFileName(aitexture->mFilename.C_Str());

			// テクスチャファイルを読み込みSRVを生成
			// SRV
			ID3D11ShaderResourceView* srv = nullptr;
			//			srv = LoadDiffuseTexture(filename);

			bool sts = CreateSRVfromFile(
				filename.c_str(),
				device,
				devicecontext,
				&srv);

			if (sts) {
				// assimpの内部データにセットされているテクスチャ名をキーとして保存
				m_Texturedic[aitexture->mFilename.data] = srv;
			}
			else {
				m_Texturedic[aitexture->mFilename.data] = nullptr;
			}
		}
	}
}

// 内蔵テクスチャかどうかを識別する
bool CStaticMesh::isEmbeddedTexure(const aiTexture* aitexture) {
	if (aitexture->mHeight == 0 ||
		aitexture->mWidth == 0 ||
		aitexture->pcData == nullptr) {
		return true;
	}
	return false;
}

// 個々のマテリアル情報（反射情報）取得
void CStaticMesh::GetaMatreial(const aiMaterial* aimtrl) {
	// デバイス、デバイスコンテキスト取得
	ID3D11DeviceContext* devicecontext;
	ID3D11Device* device;

	//		device = CDirectXGraphics::GetInstance()->GetDXDevice();
	//		devicecontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	device = Renderer::GetDevice();
	devicecontext = Renderer::GetDeviceContext();

	// マテリアル情報
	MATERIAL mymaterial{};

	// 拡散反射成分
	{
		aiColor3D color(0.0f, 0.0f, 0.0f);

		if (aimtrl->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS) {
			mymaterial.Diffuse.x = color.r;
			mymaterial.Diffuse.y = color.g;
			mymaterial.Diffuse.z = color.b;
			mymaterial.Diffuse.w = 1.0f;
		}
		else {
			mymaterial.Diffuse.x = 0.5f;
			mymaterial.Diffuse.y = 0.5f;
			mymaterial.Diffuse.z = 0.5f;
			mymaterial.Diffuse.w = 1.0f;
		}
	}

	// 鏡面反射成分
	{
		aiColor3D color(0.0f, 0.0f, 0.0f);

		if (aimtrl->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS) {
			mymaterial.Specular.x = color.r;
			mymaterial.Specular.y = color.g;
			mymaterial.Specular.z = color.b;
			mymaterial.Specular.w = 1.0f;
		}
		else {
			mymaterial.Specular.x = 0.0f;
			mymaterial.Specular.y = 0.0f;
			mymaterial.Specular.z = 0.0f;
			mymaterial.Specular.w = 0.0f;
		}
	}

	// 鏡面反射強度
	{
		float Shinness = 0.0f;

		if (aimtrl->Get(AI_MATKEY_SHININESS, Shinness) == AI_SUCCESS) {
			mymaterial.Shininess = Shinness;
		}
		else {
			mymaterial.Shininess = 0.0f;
		}
	}

	// 環境光反射成分
	{
		aiColor3D color(0.0f, 0.0f, 0.0f);

		if (aimtrl->Get(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS) {
			mymaterial.Ambient.x = color.r;
			mymaterial.Ambient.y = color.g;
			mymaterial.Ambient.z = color.b;
			mymaterial.Ambient.w = 1.0f;
		}
		else {
			mymaterial.Ambient.x = 0.0f;
			mymaterial.Ambient.y = 0.0f;
			mymaterial.Ambient.z = 0.0f;
			mymaterial.Ambient.w = 0.0f;
		}
	}

	// 自家発光成分
	{
		aiColor3D color(0.0f, 0.0f, 0.0f);

		if (aimtrl->Get(AI_MATKEY_COLOR_EMISSIVE, color) == AI_SUCCESS) {
			mymaterial.Emission.x = color.r;
			mymaterial.Emission.y = color.g;
			mymaterial.Emission.z = color.b;
			mymaterial.Emission.w = 1.0f;
		}
		else {
			mymaterial.Emission.x = 0.0f;
			mymaterial.Emission.y = 0.0f;
			mymaterial.Emission.z = 0.0f;
			mymaterial.Emission.w = 0.0f;
		}
	}

	// ディフューズテクスチャ数取得
	aiTextureType type = aiTextureType_DIFFUSE;
	int texnum = aimtrl->GetTextureCount(type);

	// 1メッシュにディフーズテクスチャ１枚しか許可せず
	assert(texnum <= 1);

	// マテリアル名
	aiString str;

	// assimpからテクスチャファイル名を取得
	aimtrl->GetTexture(type, 0, &str);

	// SRV
	ID3D11ShaderResourceView* srv = nullptr;

	// SRV辞書に存在するか？
	if (m_Texturedic.find(str.data) == m_Texturedic.end()) {
		mymaterial.TextureEnable = FALSE;
		srv = nullptr;

		// pmx の場合シーンに登録されていないテクスチャが存在する
		aimtrl->GetTexture(type, 0, &str);

		// ファイル名を取得する
		std::string filename = GetFileName(str.C_Str());

		// テクスチャを読み込み取得
		// SRV
		srv = LoadDiffuseTexture(filename);

		if (srv == nullptr) {
			mymaterial.TextureEnable = FALSE;
		}
		else {
			mymaterial.TextureEnable = TRUE;
		}
	}
	else {
		mymaterial.TextureEnable = TRUE;
		srv = m_Texturedic[str.data];
	}

	// SRV保存
	m_AllTexture.emplace_back(srv);

	// マテリアル情報を保存
	m_materials.emplace_back(mymaterial);
}

// マテリアル取得
void CStaticMesh::GetMaterials(const aiScene* aiscene) {
	// このモデルで使用しているすべてのマテリアル情報を取得
	for (unsigned int mrlidx = 0; mrlidx < aiscene->mNumMaterials; mrlidx++) {
		// このメッシュ用のマテリアルを取得
		aiMaterial* mtl;
		mtl = aiscene->mMaterials[mrlidx];

		// マテリアル情報取得
		GetaMatreial(mtl);
	}
}

// diffuse texture load
ID3D11ShaderResourceView* CStaticMesh::LoadDiffuseTexture(std::string filename) {
	// ファイルの拡張子を取得
	std::string fileext = GetFileExt(filename);

	// ファイル名(UTF-16)を取得する
	std::wstring widefilename = GetFileNameWide(filename.c_str());

	// モデルデータの親ディレクトリを取得
	std::wstring parentdirectory = GetParentDirectoryWide(m_filename);// +L"";

	// テクスチャのパス名取得
	std::wstring filepath = parentdirectory + widefilename;

	// SRV
	ID3D11ShaderResourceView* srv = nullptr;

	// TGA？
	if (fileext == ".tga") {
		// TGAの場合
		DirectX::TexMetadata meta;
		DirectX::GetMetadataFromTGAFile(filepath.c_str(), meta);

		std::unique_ptr<DirectX::ScratchImage> image(new DirectX::ScratchImage);

		HRESULT hr = LoadFromTGAFile(filepath.c_str(), &meta, *image);
		if (FAILED(hr)) {
			std::cout << "LoadFromTGAFile Error (" << filename << ")" << std::endl;
			return nullptr;
		}

		ID3D11Resource* tex = nullptr;

		// テクスチャ生成
		hr = CreateTexture(
			Renderer::GetDevice(),
			image->GetImages(),
			image->GetImageCount(),
			meta,
			&tex);

		tex->Release();

		if (FAILED(hr)) {
			std::cout << "CreateTexture Error (" << filename << ")" << std::endl;
			return nullptr;
		}

		// シェーダーリソースビュー作成
		hr = DirectX::CreateShaderResourceView(Renderer::GetDevice(),
			image->GetImages(),
			image->GetImageCount(),
			meta,
			&srv);

		if (FAILED(hr)) {
			std::wcout << L"CreateShaderResourceView Error (" << filepath.c_str() << L")" << std::endl;
			return nullptr;
		}
	}
	else {
		// テクスチャ読み込み
		HRESULT hr = DirectX::CreateWICTextureFromFile(
			Renderer::GetDevice(),
			filepath.c_str(),
			nullptr,
			&srv);
		if (FAILED(hr)) {
			std::wcout << L"CreateWICTextureFromFile Error (" << filepath.c_str() << widefilename.c_str() << L")" << std::endl;
			return nullptr;
		}
	}

	return srv;
}

// ボーン情報生成（20231220追加）(スタティックメッシュ内の全てのボーン辞書を生成する)
void CStaticMesh::CreateBone(aiNode* node)
{
	C_BONE bone{};

	// ボーン名で参照できるように空のボーン情報をセットする
	m_Bone[node->mName.C_Str()] = bone;

	for (unsigned int n = 0; n < node->mNumChildren; n++)
	{
		CreateBone(node->mChildren[n]);
	}
}

// サブセットに紐づいているボーン情報を取得する
std::vector<C_BONE> CStaticMesh::GetBoneInfo(const aiMesh* mesh) {
	std::vector<C_BONE> bones;		// このサブセットメッシュで使用されているボーンコンテナ

	// ボーン数分ループ
	for (unsigned int bidx = 0; bidx < mesh->mNumBones; bidx++) {
		C_BONE bone{};

		// ボーン名取得
		bone.bonename = std::string(mesh->mBones[bidx]->mName.C_Str());
		// メッシュノード名
		bone.meshname = std::string(mesh->mBones[bidx]->mNode->mName.C_Str());
		// アーマチュアノード名
		bone.armaturename = std::string(mesh->mBones[bidx]->mArmature->mName.C_Str());

		// デバッグ用
		std::cout << bone.bonename
			<< "(" << bone.meshname << ")"
			<< "(" << bone.armaturename << ")"
			<< std::endl;

		// ボーンオフセット行列取得
		bone.OffsetMatrix = mesh->mBones[bidx]->mOffsetMatrix;

		// ウェイト情報抽出
		bone.weights.clear();
		for (unsigned int widx = 0; widx < mesh->mBones[bidx]->mNumWeights; widx++) {
			WEIGHT w;
			w.meshname = bone.meshname;										// メッシュ名
			w.bonename = bone.bonename;										// ボーン名

			w.weight = mesh->mBones[bidx]->mWeights[widx].mWeight;			// 重み
			w.vertexindex = mesh->mBones[bidx]->mWeights[widx].mVertexId;	// 頂点インデックス
			bone.weights.emplace_back(w);
		}

		// コンテナに登録
		bones.emplace_back(bone);

		// ボーン辞書にも反映させる
		m_Bone[mesh->mBones[bidx]->mName.C_Str()].OffsetMatrix = mesh->mBones[bidx]->mOffsetMatrix;
	}

	return bones;
}

// ボーン名、ボーンインデックス、ボーンウェイトを頂点にセットする（20231225追加）
void CStaticMesh::SetBoneDataToVertices() {
	// ボーンインデックスを初期化
	for (auto& v : m_vertices) {
		v.bonecnt = 0;
		for (int b = 0; b < 4; b++) {
			v.BoneIndex[b] = -1;
			v.BoneWeight[b] = 0.0f;
			//			v.BoneName[b] = "";
		}
	}

	// サブセットメッシュ毎のコンテナ
	int subsetid = 0;
	for (auto& bones : m_SubsetBones) {
		// このスタティックメッシュ内の頂点データのスタート位置を取得
		int vertexbase = m_subsets[subsetid].VertexBase;

		// このサブセット内のボーンをひとつづつ取り出す
		for (auto& bone : bones)
		{
			for (auto& w : bone.weights) {
				int& idx = m_vertices[vertexbase + w.vertexindex].bonecnt;
				//				m_vertices[vertexbase + w.vertexindex].BoneName[idx] = w.bonename;	// ボーン名をセット
				m_vertices[vertexbase + w.vertexindex].BoneWeight[idx] = w.weight;	// weight値をセット
				m_vertices[vertexbase + w.vertexindex].BoneIndex[idx] = m_Bone[w.bonename].idx;
				//ボーンの配列番号をセット
				idx++;
				assert(idx <= 4);
			}
		}
		subsetid++;				// 次のサブセットへ
	}
}