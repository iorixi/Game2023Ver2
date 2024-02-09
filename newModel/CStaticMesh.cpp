#include	<iostream>
#include	"CStaticMesh.h"
#include	"../renderer.h"

using namespace DirectX;

// ���f���t�@�C��������������
bool CStaticMesh::Init(
	std::string filename)			//	�R�c���f���t�@�C��
{
	bool sts{};

	m_filename = filename;			// �e�N�X�`���ǂݍ��ݗp�ɕۑ�

	// �V�[�����\�z
	static Assimp::Importer importer;

	// �V�[�������\�z
	m_AiScene = importer.ReadFile(
		filename.c_str(),
		//			aiProcess_ConvertToLeftHanded | aiProcess_Triangulate);
		aiProcessPreset_TargetRealtime_MaxQuality |
		aiProcess_ConvertToLeftHanded |
		aiProcess_PopulateArmatureData);		// 20231225�ǉ�

	//			aiProcess_ConvertToLeftHanded |		// ������W�n�ɕϊ�����
	//		aiProcess_PopulateArmatureData |
	//		aiProcess_PreTransformVertices |
			// �m�[�h�\�����폜���ā@�����̃m�[�h�������[�J���ϊ��s��ł��ׂĂ̒��_�����O�ɕϊ�����
	//			aiProcess_Triangulate);
			// �O�p�`������
	//		aiProcess_GenSmoothNormals |
	//		aiProcess_JoinIdenticalVertices);

	if (m_AiScene == nullptr) {
		printf("load error %s ( %s ) \n ", filename.c_str(), importer.GetErrorString());
		return false;
	}

	//�ċA�I�Ƀ{�[�������i��ŊK�w�\�������j 20231226�ǉ�
	CreateBone(m_AiScene->mRootNode);

	// �{�[���̔z��ʒu���i�[����				// 20231226
	unsigned int num = 0;						// 20231226
	for (auto& data : m_Bone) {					// 20231226
		data.second.idx = num;					// 20231226
		num++;									// 20231226
	}											// 20231226

	// ���b�V�����Ń��b�V���R���e�i�z�񐔂�����
	m_subsets.resize(m_AiScene->mNumMeshes);

	// ���b�V���������[�v
	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++) {
		aiMesh* mesh{};
		mesh = m_AiScene->mMeshes[m];

		// ���b�V���P�ʂŒ��_�f�[�^���擾
		int vnum = GetVerticesFromaiMesh(mesh);

		// ���b�V���P�ʂŃC���f�b�N�X�f�[�^�擾
		int idxnum = GetIndicesFromaiMesh(mesh);

		// ���b�V���Ŏg�p���Ă���}�e���A�������擾
		int mtrlidx = mesh->mMaterialIndex;

		// �T�u�Z�b�g�����Z�b�g����
		m_subsets[m].Name = std::string(mesh->mName.C_Str());		// 20231225�ǉ�

		// �C���f�b�N�X����ۑ�
		m_subsets[m].IndexNum = idxnum;

		// ���_����ۑ�
		m_subsets[m].VertexNum = vnum;

		// �}�e���A���C���f�b�N�X��ۑ�
		m_subsets[m].MaterialIdx = mtrlidx;

		// �{�[�������擾����(�T�u�Z�b�g�P�ʂ�)
//			std::vector<C_BONE> subsetbone = GetBoneInfo(mesh);  for debug
		m_SubsetBones.emplace_back(GetBoneInfo(mesh));
	}

	// �T�u�Z�b�g���P�̒��_�o�b�t�@�E�C���f�b�N�X�o�b�t�@
	// �ɓ������Ă���̂Ńx�[�X���i�X�^�[�g�ʒu�j
	// ���v�Z�ŋ��߂Ă���
	CalcMeshBaseIndex();

	// �{�[��ID�ƃE�G�C�g�l�𒸓_�f�[�^�ɔ��f������
	// �K��CalcMeshBaseIndex()�̌�ōs����
	SetBoneDataToVertices();

	// �S�e�N�X�`�����擾
	GetTextures(m_AiScene);

	// �}�e���A�����擾
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
	// SRV���
//	for (auto& srv : m_AllTexture) {
//		srv->Release();
//	}

	// SRV���
	for (auto& texres : m_Texturedic) {
		if (texres.second != nullptr) {
			texres.second->Release();
		}
	}
}

// �S���_��aiMesh����擾����
unsigned int  CStaticMesh::GetVerticesFromaiMesh(const aiMesh* aimesh) {
	// ���_�������[�v
	for (unsigned int vidx = 0; vidx < aimesh->mNumVertices; vidx++) {
		VERTEX_3D v;

		v.Position.x = aimesh->mVertices[vidx].x;
		v.Position.y = aimesh->mVertices[vidx].y;
		v.Position.z = aimesh->mVertices[vidx].z;

		// �@������H
		if (aimesh->HasNormals()) {
			v.Normal.x = aimesh->mNormals[vidx].x;
			v.Normal.y = aimesh->mNormals[vidx].y;
			v.Normal.z = aimesh->mNormals[vidx].z;
		}
		else {
			v.Normal = XMFLOAT3(0, 0, 0);
		}

		// �e�N�X�`������H
		if (aimesh->HasTextureCoords(0)) {
			v.TexCoord.x = aimesh->mTextureCoords[0][vidx].x;				// 20230809 bug fix by ts
			v.TexCoord.y = aimesh->mTextureCoords[0][vidx].y;				// 20230809 bug fix by ts
		}
		else {
			v.TexCoord = XMFLOAT2(0, 0);
		}

		// ���_�J���[����H
		if (aimesh->HasVertexColors(0)) {
			v.Diffuse.x = aimesh->mColors[0][vidx].r;
			v.Diffuse.y = aimesh->mColors[0][vidx].g;
			v.Diffuse.z = aimesh->mColors[0][vidx].b;
			v.Diffuse.w = aimesh->mColors[0][vidx].a;
		}
		else {
			v.Diffuse = XMFLOAT4(1, 1, 1, 1);		// ���������ꍇ�͔�
		}

		// ���_�f�[�^��ۑ�
		m_vertices.emplace_back(v);
	}

	// aiMesh���ێ����钸�_����߂�
	return aimesh->mNumVertices;
}

// aiMesh����S�C���f�b�N�X�擾
unsigned int  CStaticMesh::GetIndicesFromaiMesh(const aiMesh* aimesh) {
	// �ʐ������[�v
	for (unsigned int f = 0; f < aimesh->mNumFaces; f++)
	{
		const aiFace* face = &aimesh->mFaces[f];
		assert(face->mNumIndices <= 3);

		// �C���f�b�N�X�f�[�^��ۑ�
		m_indices.emplace_back(face->mIndices[0]);
		m_indices.emplace_back(face->mIndices[1]);
		m_indices.emplace_back(face->mIndices[2]);
	}

	return aimesh->mNumFaces * 3;
}

// �x�[�X�����Čv�Z
void CStaticMesh::CalcMeshBaseIndex() {
	// �T�u�Z�b�g�������[�v
	for (int m = 0; m < m_subsets.size(); m++)
	{
		// ���_�o�b�t�@�̃x�[�X���v�Z
		m_subsets[m].VertexBase = 0;
		for (int i = m - 1; i >= 0; i--) {
			m_subsets[m].VertexBase += m_subsets[i].VertexNum;
		}

		// �C���f�b�N�X�o�b�t�@�̃x�[�X���v�Z
		m_subsets[m].IndexBase = 0;
		for (int i = m - 1; i >= 0; i--) {
			m_subsets[m].IndexBase += m_subsets[i].IndexNum;
		}
	}
}

// ���ׂẴe�N�X�`��SRV���擾
void CStaticMesh::GetTextures(const aiScene* aiscene) {
	ID3D11DeviceContext* devicecontext;
	ID3D11Device* device;

	//		device = CDirectXGraphics::GetInstance()->GetDXDevice();
	//		devicecontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	device = Renderer::GetDevice();
	devicecontext = Renderer::GetDeviceContext();

	// ���̃V�[�������e�N�X�`����S�ēǂݍ���
	for (unsigned int texidx = 0; texidx < aiscene->mNumTextures; texidx++) {
		// �e�N�X�`�����擾
		aiTexture* aitexture = aiscene->mTextures[texidx];

		// �����e�N�X�`�����H
		if (isEmbeddedTexure(aitexture)) {
			ID3D11ShaderResourceView* srv;

			aiTexture* aitexture = aiscene->mTextures[texidx];

			// mHeight is 0, so try to load a compressed texture of mWidth bytes
			const size_t size = aitexture->mWidth;

			// SRV����
			HRESULT hr = DirectX::CreateWICTextureFromMemory(
				device,
				devicecontext,
				reinterpret_cast<const unsigned char*>(aitexture->pcData),
				aitexture->mWidth,
				nullptr,
				&srv);

			assert(srv);

			// assimp�̓����f�[�^�ɃZ�b�g����Ă���e�N�X�`�������L�[�Ƃ��ĕۑ�
			m_Texturedic[aitexture->mFilename.data] = srv;
		}
		else {
			// �����e�N�X�`���ł͂Ȃ��ꍇ

			// �e�N�X�`���t�@�C�������擾����
			std::string filename = GetFileName(aitexture->mFilename.C_Str());

			// �e�N�X�`���t�@�C����ǂݍ���SRV�𐶐�
			// SRV
			ID3D11ShaderResourceView* srv = nullptr;
			//			srv = LoadDiffuseTexture(filename);

			bool sts = CreateSRVfromFile(
				filename.c_str(),
				device,
				devicecontext,
				&srv);

			if (sts) {
				// assimp�̓����f�[�^�ɃZ�b�g����Ă���e�N�X�`�������L�[�Ƃ��ĕۑ�
				m_Texturedic[aitexture->mFilename.data] = srv;
			}
			else {
				m_Texturedic[aitexture->mFilename.data] = nullptr;
			}
		}
	}
}

// �����e�N�X�`�����ǂ��������ʂ���
bool CStaticMesh::isEmbeddedTexure(const aiTexture* aitexture) {
	if (aitexture->mHeight == 0 ||
		aitexture->mWidth == 0 ||
		aitexture->pcData == nullptr) {
		return true;
	}
	return false;
}

// �X�̃}�e���A�����i���ˏ��j�擾
void CStaticMesh::GetaMatreial(const aiMaterial* aimtrl) {
	// �f�o�C�X�A�f�o�C�X�R���e�L�X�g�擾
	ID3D11DeviceContext* devicecontext;
	ID3D11Device* device;

	//		device = CDirectXGraphics::GetInstance()->GetDXDevice();
	//		devicecontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	device = Renderer::GetDevice();
	devicecontext = Renderer::GetDeviceContext();

	// �}�e���A�����
	MATERIAL mymaterial{};

	// �g�U���ː���
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

	// ���ʔ��ː���
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

	// ���ʔ��ˋ��x
	{
		float Shinness = 0.0f;

		if (aimtrl->Get(AI_MATKEY_SHININESS, Shinness) == AI_SUCCESS) {
			mymaterial.Shininess = Shinness;
		}
		else {
			mymaterial.Shininess = 0.0f;
		}
	}

	// �������ː���
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

	// ���Ɣ�������
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

	// �f�B�t���[�Y�e�N�X�`�����擾
	aiTextureType type = aiTextureType_DIFFUSE;
	int texnum = aimtrl->GetTextureCount(type);

	// 1���b�V���Ƀf�B�t�[�Y�e�N�X�`���P������������
	assert(texnum <= 1);

	// �}�e���A����
	aiString str;

	// assimp����e�N�X�`���t�@�C�������擾
	aimtrl->GetTexture(type, 0, &str);

	// SRV
	ID3D11ShaderResourceView* srv = nullptr;

	// SRV�����ɑ��݂��邩�H
	if (m_Texturedic.find(str.data) == m_Texturedic.end()) {
		mymaterial.TextureEnable = FALSE;
		srv = nullptr;

		// pmx �̏ꍇ�V�[���ɓo�^����Ă��Ȃ��e�N�X�`�������݂���
		aimtrl->GetTexture(type, 0, &str);

		// �t�@�C�������擾����
		std::string filename = GetFileName(str.C_Str());

		// �e�N�X�`����ǂݍ��ݎ擾
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

	// SRV�ۑ�
	m_AllTexture.emplace_back(srv);

	// �}�e���A������ۑ�
	m_materials.emplace_back(mymaterial);
}

// �}�e���A���擾
void CStaticMesh::GetMaterials(const aiScene* aiscene) {
	// ���̃��f���Ŏg�p���Ă��邷�ׂẴ}�e���A�������擾
	for (unsigned int mrlidx = 0; mrlidx < aiscene->mNumMaterials; mrlidx++) {
		// ���̃��b�V���p�̃}�e���A�����擾
		aiMaterial* mtl;
		mtl = aiscene->mMaterials[mrlidx];

		// �}�e���A�����擾
		GetaMatreial(mtl);
	}
}

// diffuse texture load
ID3D11ShaderResourceView* CStaticMesh::LoadDiffuseTexture(std::string filename) {
	// �t�@�C���̊g���q���擾
	std::string fileext = GetFileExt(filename);

	// �t�@�C����(UTF-16)���擾����
	std::wstring widefilename = GetFileNameWide(filename.c_str());

	// ���f���f�[�^�̐e�f�B���N�g�����擾
	std::wstring parentdirectory = GetParentDirectoryWide(m_filename);// +L"";

	// �e�N�X�`���̃p�X���擾
	std::wstring filepath = parentdirectory + widefilename;

	// SRV
	ID3D11ShaderResourceView* srv = nullptr;

	// TGA�H
	if (fileext == ".tga") {
		// TGA�̏ꍇ
		DirectX::TexMetadata meta;
		DirectX::GetMetadataFromTGAFile(filepath.c_str(), meta);

		std::unique_ptr<DirectX::ScratchImage> image(new DirectX::ScratchImage);

		HRESULT hr = LoadFromTGAFile(filepath.c_str(), &meta, *image);
		if (FAILED(hr)) {
			std::cout << "LoadFromTGAFile Error (" << filename << ")" << std::endl;
			return nullptr;
		}

		ID3D11Resource* tex = nullptr;

		// �e�N�X�`������
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

		// �V�F�[�_�[���\�[�X�r���[�쐬
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
		// �e�N�X�`���ǂݍ���
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

// �{�[����񐶐��i20231220�ǉ��j(�X�^�e�B�b�N���b�V�����̑S�Ẵ{�[�������𐶐�����)
void CStaticMesh::CreateBone(aiNode* node)
{
	C_BONE bone{};

	// �{�[�����ŎQ�Ƃł���悤�ɋ�̃{�[�������Z�b�g����
	m_Bone[node->mName.C_Str()] = bone;

	for (unsigned int n = 0; n < node->mNumChildren; n++)
	{
		CreateBone(node->mChildren[n]);
	}
}

// �T�u�Z�b�g�ɕR�Â��Ă���{�[�������擾����
std::vector<C_BONE> CStaticMesh::GetBoneInfo(const aiMesh* mesh) {
	std::vector<C_BONE> bones;		// ���̃T�u�Z�b�g���b�V���Ŏg�p����Ă���{�[���R���e�i

	// �{�[���������[�v
	for (unsigned int bidx = 0; bidx < mesh->mNumBones; bidx++) {
		C_BONE bone{};

		// �{�[�����擾
		bone.bonename = std::string(mesh->mBones[bidx]->mName.C_Str());
		// ���b�V���m�[�h��
		bone.meshname = std::string(mesh->mBones[bidx]->mNode->mName.C_Str());
		// �A�[�}�`���A�m�[�h��
		bone.armaturename = std::string(mesh->mBones[bidx]->mArmature->mName.C_Str());

		// �f�o�b�O�p
		std::cout << bone.bonename
			<< "(" << bone.meshname << ")"
			<< "(" << bone.armaturename << ")"
			<< std::endl;

		// �{�[���I�t�Z�b�g�s��擾
		bone.OffsetMatrix = mesh->mBones[bidx]->mOffsetMatrix;

		// �E�F�C�g��񒊏o
		bone.weights.clear();
		for (unsigned int widx = 0; widx < mesh->mBones[bidx]->mNumWeights; widx++) {
			WEIGHT w;
			w.meshname = bone.meshname;										// ���b�V����
			w.bonename = bone.bonename;										// �{�[����

			w.weight = mesh->mBones[bidx]->mWeights[widx].mWeight;			// �d��
			w.vertexindex = mesh->mBones[bidx]->mWeights[widx].mVertexId;	// ���_�C���f�b�N�X
			bone.weights.emplace_back(w);
		}

		// �R���e�i�ɓo�^
		bones.emplace_back(bone);

		// �{�[�������ɂ����f������
		m_Bone[mesh->mBones[bidx]->mName.C_Str()].OffsetMatrix = mesh->mBones[bidx]->mOffsetMatrix;
	}

	return bones;
}

// �{�[�����A�{�[���C���f�b�N�X�A�{�[���E�F�C�g�𒸓_�ɃZ�b�g����i20231225�ǉ��j
void CStaticMesh::SetBoneDataToVertices() {
	// �{�[���C���f�b�N�X��������
	for (auto& v : m_vertices) {
		v.bonecnt = 0;
		for (int b = 0; b < 4; b++) {
			v.BoneIndex[b] = -1;
			v.BoneWeight[b] = 0.0f;
			//			v.BoneName[b] = "";
		}
	}

	// �T�u�Z�b�g���b�V�����̃R���e�i
	int subsetid = 0;
	for (auto& bones : m_SubsetBones) {
		// ���̃X�^�e�B�b�N���b�V�����̒��_�f�[�^�̃X�^�[�g�ʒu���擾
		int vertexbase = m_subsets[subsetid].VertexBase;

		// ���̃T�u�Z�b�g���̃{�[�����ЂƂÂ��o��
		for (auto& bone : bones)
		{
			for (auto& w : bone.weights) {
				int& idx = m_vertices[vertexbase + w.vertexindex].bonecnt;
				//				m_vertices[vertexbase + w.vertexindex].BoneName[idx] = w.bonename;	// �{�[�������Z�b�g
				m_vertices[vertexbase + w.vertexindex].BoneWeight[idx] = w.weight;	// weight�l���Z�b�g
				m_vertices[vertexbase + w.vertexindex].BoneIndex[idx] = m_Bone[w.bonename].idx;
				//�{�[���̔z��ԍ����Z�b�g
				idx++;
				assert(idx <= 4);
			}
		}
		subsetid++;				// ���̃T�u�Z�b�g��
	}
}