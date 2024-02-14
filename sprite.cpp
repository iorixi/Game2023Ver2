#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "sprite.h"

using namespace DirectX::SimpleMath;

void Sprite::UpdateSpriteBase()
{
	VERTEX_3D vertex[4];

	vertex[0].Position = Vector3((float)spriteDate.x, (float)spriteDate.y, 0.0f);
	vertex[0].Normal = Vector3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = Vector2(0.0f, 0.0f);

	vertex[1].Position = Vector3((float)(spriteDate.x + spriteDate.width), (float)spriteDate.y, 0.0f);
	vertex[1].Normal = Vector3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = Vector2(1.0f, 0.0f);

	vertex[2].Position = Vector3((float)spriteDate.x, (float)(spriteDate.y + spriteDate.height), 0.0f);
	vertex[2].Normal = Vector3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = Vector2(0.0f, 1.0f);

	vertex[3].Position = Vector3((float)(spriteDate.x + spriteDate.width), (float)(spriteDate.y + spriteDate.height), 0.0f);
	vertex[3].Normal = Vector3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = Vector2(1.0f, 1.0f);

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
}

void Sprite::Init(int x, int y, int Width, int Height, const char* TextureName)
{
	spriteDate.x = x;
	spriteDate.y = y;
	spriteDate.width = Width;
	spriteDate.height = Height;

	UpdateSpriteBase();

	std::wstring ws = sjis_to_wide_winapi(TextureName);

	// テクスチャ読み込み
	DirectX::CreateWICTextureFromFile(
		Renderer::GetDevice(),
		ws.c_str(),
		nullptr,
		//		&m_TextureResource,
		&m_Texture);

	assert(m_Texture);

	m_Material.Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_Material.TextureEnable = true;
}

void Sprite::Init(int x, int y, int Width, int Height, const char* TextureName, float r, float g, float b, float a)
{
	spriteDate.x = x;
	spriteDate.y = y;
	spriteDate.width = Width;
	spriteDate.height = Height;

	UpdateSpriteBase();

	std::wstring ws = sjis_to_wide_winapi(TextureName);

	// テクスチャ読み込み
	DirectX::CreateWICTextureFromFile(
		Renderer::GetDevice(),
		ws.c_str(),
		nullptr,
		//		&m_TextureResource,
		&m_Texture);

	assert(m_Texture);

	m_Material.Diffuse = Color(r, g, b, a);
	m_Material.TextureEnable = true;
}

void Sprite::Update()
{
	UpdateSpriteBase();

	m_Material.TextureEnable = true;
}

void Sprite::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();
	//	m_TextureResource->Release();
}

void Sprite::Draw()
{
	if (id == 777)
	{
		int a = 0;
	}

	// マトリクス設定
	Renderer::SetWorldViewProjection2D();

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// マテリアル設定
	Renderer::SetMaterial(m_Material);

	// ポリゴン描画
	Renderer::GetDeviceContext()->Draw(4, 0);
}

void Sprite::SetPos(float x, float y)
{
	spriteDate.x = x;
	spriteDate.y = y;
}

void Sprite::SetSpriteData(SpriteData _spriteData)
{
	spriteDate = _spriteData;
}

float Sprite::GetPosX()
{
	return spriteDate.x;
}

float Sprite::GetPosY()
{
	return spriteDate.y;
}