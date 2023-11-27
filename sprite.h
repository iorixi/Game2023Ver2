#pragma once

#include	"renderer.h"
#include	"component.h"
#include	"utftosjisconv.h"

class Sprite : public Component
{

private:
	ID3D11Buffer*				m_VertexBuffer{};
	ID3D11ShaderResourceView*	m_Texture{};
//	ID3D11Resource*				m_TextureResource{};

	MATERIAL					m_Material{};

public:

	using Component::Component;

	void Init(int x, int y, int Width, int Height, const char* TextureName);
	void Uninit() override;
	void Draw() override;

	void SetMaterial(MATERIAL Material) { m_Material = Material; }

};