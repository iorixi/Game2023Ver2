#pragma once
#include	<simplemath.h>
#include	<random>
#include	<iostream>
#include	<algorithm>
#include	"CMesh.h"

class CPlaneMesh : public CMesh {
public:
	struct FACE {
		unsigned int idx[3];
	};

	void Init(
		int divx, int divy,
		int width, int height,
		DirectX::SimpleMath::Color color,
		DirectX::SimpleMath::Vector3 normal,
		bool xzflag = false);
	void CreateVertex();
	void CreateVertexXZ();
	void CreateIndex();
	// ��
	float GetWidth();
	// ����
	float GetHeight();
	int GetDivX();
	int GetDivY();

	// �w�肵��3�p�`�ԍ��̎O�p�`�C���f�b�N�X���擾
	FACE GetTriangle(int triangleno);

public:

	// ���Ԗڂ̎l�p�`����������
	int GetSquareNo(DirectX::SimpleMath::Vector3 pos);

private:
	unsigned int m_divX = 1;
	unsigned int m_divY = 1;
	float  m_height = 100.0f;
	float  m_width = 100.0f;
	DirectX::SimpleMath::Color m_color;
	DirectX::SimpleMath::Vector3 m_normal;
};



class CPlaneMeshWithoutCommon : public CMesh {
public:
	void Init(int divx, int divy,
		int width, int height,
		DirectX::SimpleMath::Color color,
		DirectX::SimpleMath::Vector3 normal);
	void CreateIndex();
	// ��
	float GetWidth();
	// ����
	float GetHeight();
	int GetDivX();
	int GetDivY();

private:
	int m_divX = 1;
	int m_divY = 1;
	float  m_height = 100.0f;
	float  m_width = 100.0f;
	DirectX::SimpleMath::Color m_color;
	DirectX::SimpleMath::Vector3 m_normal;
};
