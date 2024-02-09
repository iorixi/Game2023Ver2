#pragma once
#include	<d3d11.h>
#include	<DirectXMath.h>
#include	<wrl/client.h>
#include	<vector>
#include	<string>
#include	<unordered_map>
#include	<WICTextureLoader.h>

// assimp�T�D�Q�D�T�p
#include	<assimp\Importer.hpp>
#include	<assimp\scene.h>
#include	<assimp\postprocess.h>
#include	<assimp/cimport.h>

// ���_��`
#include	"../renderer.h"

#include	"dx11helper.h"
#include	"../utility.h"
#include	"CMaterial.h"
#include	"../CMesh.h"
#include "../modelRenderer.h"

// assimp 5.2.5 �p���C�u����
#pragma comment(lib,"assimp-vc143-mtd.lib")

class CStaticMesh : public CMesh {
private:
	std::vector<SUBSET>			m_subsets;		// �T�u�Z�b�g�Q
	std::vector<MATERIAL>		m_materials;	// �}�e���A���e�[�u��

	// �����e�N�X�`�����ǂ��������ʂ���
	bool isEmbeddedTexure(const aiTexture* aitexture);

	// assimp�̓����f�[�^�ɃZ�b�g����Ă���e�N�X�`�������L�[�Ƃ��ĕۑ�
	std::unordered_map<std::string, ID3D11ShaderResourceView*> m_Texturedic;

	// �e�N�X�`�����\�[�X�e�[�u��
	std::vector<ID3D11ShaderResourceView*>	m_AllTexture;

	// �e�N�X�`���t�@�C����ǂݍ����SRV�𐶐�����
	ID3D11ShaderResourceView* LoadDiffuseTexture(std::string filename);

	// ���̃��f���̃p�X��
	std::string m_filename;

protected:
	const aiScene* m_AiScene = nullptr;			// assimp scene

	// �{�[����񎫏��i�{�[�����ŎQ�Ɖ\�j					// �i20231220�ǉ��j
	std::unordered_map<std::string, C_BONE> m_Bone;			// �i20231220�ǉ��j

	// �T�u�Z�b�g�P�ʂŃ{�[�������W�߂�����
	std::vector<std::vector<C_BONE>>	m_SubsetBones;			// �i20231225�ǉ��j

	// �{�[����񐶐��i�{�[���̋���𐶐�����j			// �i20231220�ǉ��j
	void CreateBone(aiNode* node);

	// �{�[�����A�{�[���E�F�C�g�𒸓_�ɃZ�b�g����
	void SetBoneDataToVertices();							// �i20231225�ǉ��j

	// �{�[�������擾����
	std::vector<C_BONE> GetBoneInfo(const aiMesh* mesh);		//	(20231225�ǉ�)

	// �T�u�Z�b�g���P�ɂ܂Ƃ߂邽�߂̃x�[�X���v�Z����
	void CalcMeshBaseIndex();

	// �S�e�N�X�`�����擾(�������쐬�����)
	void GetTextures(const aiScene* aiscene);

	// �}�e���A���Ɋ܂܂��f�[�^���擾����
	void GetMaterials(const aiScene* aiscene);				// �S�}�e���A�������[�v
	void GetaMatreial(const aiMaterial* aimtrl);			// �P�̃}�e���A��

	// ai���b�V���Ɋ܂܂�钸�_�����o��
	unsigned int GetVerticesFromaiMesh(const aiMesh* aimesh);

	// ai���b�V���Ɋ܂܂��C���f�b�N�X�����o��
	unsigned int  GetIndicesFromaiMesh(const aiMesh* aimesh);
public:
	// ������
	bool Init(std::string filename);

	// �I������
	void Exit();

	// �T�u�Z�b�g�f�[�^�擾
	const std::vector<SUBSET>& GetSubsets() {
		return m_subsets;
	}

	// SRV�e�[�u���擾
	const std::vector<ID3D11ShaderResourceView*>& GetSRVtable() {
		return m_AllTexture;
	}

	// �}�e���A���f�[�^�擾
	const std::vector<MATERIAL>& GetMaterials() {
		return m_materials;
	}
};