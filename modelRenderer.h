#pragma once
#include <string>
#include <unordered_map>
#include "component.h"
#include "utftosjisconv.h"
#include "renderer.h"

// �}�e���A���\����
struct MODEL_MATERIAL
{
	char						Name[256];
	MATERIAL					Material;
	char						TextureName[256];
	ID3D11ShaderResourceView* Texture;
};

// ���b�V���i�}�e���A�����ɃT�u�Z�b�g�����݂���j
struct SUBSET {
	std::string		Name;							//�T�u�Z�b�g��
	unsigned int	IndexNum = 0;					// �C���f�b�N�X��
	unsigned int	VertexNum = 0;					// ���_��
	unsigned int	IndexBase = 0;					// �J�n�C���f�b�N�X
	unsigned int	VertexBase = 0;					// ���_�x�[�X
	unsigned int	MaterialIdx = 0;				// �}�e���A���C���f�b�N�X
	MODEL_MATERIAL	Material;
	unsigned int	StartIndex;
};

// ���f���\����
struct MODEL_OBJ
{
	VERTEX_3D* VertexArray;
	unsigned int	VertexNum;

	unsigned int* IndexArray;
	unsigned int	IndexNum;

	SUBSET* SubsetArray;
	unsigned int	SubsetNum;
};

struct MODEL
{
	ID3D11Buffer* VertexBuffer;
	ID3D11Buffer* IndexBuffer;

	SUBSET* SubsetArray;
	unsigned int	SubsetNum;
};

class ModelRenderer : public Component
{
private:

	static std::unordered_map<std::string, MODEL*> m_ModelPool;

	static void LoadModel(const char* FileName, MODEL* Model);
	static void LoadObj(const char* FileName, MODEL_OBJ* ModelObj);
	static void LoadMaterial(const char* FileName, MODEL_MATERIAL** MaterialArray, unsigned int* MaterialNum);

	MODEL* m_Model{};

public:

	static void Preload(const char* FileName);
	static void UnloadAll();

	using Component::Component;

	void Load(const char* FileName);

	void Draw() override;
};