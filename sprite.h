#pragma once

#include	"renderer.h"
#include	"component.h"
#include	"utftosjisconv.h"

//スプライトの情報を保存
struct SpriteData
{
	float x;
	float y;
	float width;
	float height;
};

class Sprite : public Component
{
private:
	ID3D11Buffer* m_VertexBuffer{};
	ID3D11ShaderResourceView* m_Texture{};
	//	ID3D11Resource*				m_TextureResource{};

	MATERIAL					m_Material{};

	SpriteData spriteDate;

	void UpdateSpriteBase();
public:

	using Component::Component;

	void Init(int x, int y, int Width, int Height, const char* TextureName);
	void Update() override;
	void Uninit() override;
	void Draw() override;
	void SetPos(float x, float y);
	void SetSpriteData(SpriteData spriteData);
	float GetPosX();
	float GetPosY();

	void SetMaterial(MATERIAL Material) { m_Material = Material; }
};