#pragma once

#include "gameObject.h"

class Score : public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer{};
	ID3D11ShaderResourceView* m_Texture{};

	int m_Count = 0;
	float m_x = 0;
	float m_y = 0;
	float m_height = 0;
	float m_width = 0;
	float m_space = 0;

public:
	void Init();
	void Init(int initCount);
	void Uninit();
	void Draw();
	void AddCount(int Count) { m_Count += Count; }
	void SubtractCount(int Count) { m_Count -= Count; }
	void SetCount(int Count) { m_Count = Count; }
	void SetPlace(float x, float y, float height, float width, float space);
};