#include <WICTextureLoader.h>
#include "renderer.h"
#include "field.h"
#include "shader.h"
#include "collision.h"

using namespace DirectX::SimpleMath;

void Field::Init()
{
	AddComponent<Shader>()->Load("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");

	// 床メッシュ生成
	m_planemesh.Init(
		100, 100,					// 分割数
		640,						// サイズ
		640,						// サイズ
		Color(1, 1, 1, 1),			// 頂点カラー
		Vector3(0, 0, -1),			// 法線ベクトル
		true);						// XZ平面

	// 凸凹を付ける（一様分布で）
//	m_planemesh.MakeUndulation(
//		0.0f,						// 最小
//		3.0f);						// 最大

	m_planemesh.MakeUndulationPerlinnoise(
		10.0f,			// 最大の高さ
		4,				// オクターブ数
		0.28f);			// パーシステンス

	// 平面の方程式を生成（全面）
	MakeEquatation();

	// レンダラー初期化
	m_renderer.Init(m_planemesh);

	// テクスチャ読み込み
	DirectX::CreateWICTextureFromFile(
		Renderer::GetDevice(),
		L"asset/texture/field.jpg",
		nullptr,
		&m_Texture);

	assert(m_Texture);
}

void Field::Uninit()
{
	for (auto& com : m_Component) {
		com->Uninit();
		delete com;
	}

	m_Component.clear();

	m_Texture->Release();
}

void Field::Update()
{
}

void Field::Draw()
{
	GetComponent<Shader>()->Draw();

	// ワールドマトリクス設定
	Matrix world, scale, rot, trans;
	scale = Matrix::CreateScale(m_Scale.x);
	rot = Matrix::CreateFromYawPitchRoll(m_Rotation.y, m_Rotation.x, m_Rotation.z);
	trans = Matrix::CreateTranslation(m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	// テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//レンダラで描画
	m_renderer.Draw();
}

// 現在位置の高さを求める
float  Field::GetFieldHeight(Vector3 pos) {
	float t;

	// 面数分
	for (unsigned int idx = 0; idx < m_planes.size(); idx++) {
		Vector3 up = { 0,1,0 };
		Vector3 startpoint = { pos.x,0,pos.z };
		Plane p = m_planes[idx].GetPlaneInfo().plane;
		Vector3 ans;

		bool sts = LinetoPlaneCross(p, startpoint, up, t, ans);
		if (sts) {
			sts = CheckInTriangle(
				m_planes[idx].GetPlaneInfo().p0,
				m_planes[idx].GetPlaneInfo().p1,
				m_planes[idx].GetPlaneInfo().p2, ans);
			if (sts) {
				return ans.y;
			}
		}
	}

	// 見つからなかった場合は０
	return 0;
}

// 現在位置の高さを求める（四角形番号を指定して）
float  Field::GetFieldHeightBySqno(Vector3 pos) {
	float t;

	// 現在位置からのっかている四角形番号を取得
	int sqno = m_planemesh.GetSquareNo(pos);

	static float oldheight = 0;

	// 下面チェック
	{
		int idx = sqno * 2;

		// 面数分
		Vector3 up = { 0,1,0 };
		Vector3 startpoint = { pos.x,0,pos.z };
		Plane p = m_planes[idx].GetPlaneInfo().plane;
		Vector3 ans;

		bool sts = LinetoPlaneCross(p, startpoint, up, t, ans);
		if (sts) {
			sts = CheckInTriangle(
				m_planes[idx].GetPlaneInfo().p0,
				m_planes[idx].GetPlaneInfo().p1,
				m_planes[idx].GetPlaneInfo().p2, ans);
			if (sts) {
				oldheight = ans.y;
				return ans.y;
			}
		}
	}

	// 上面チェック
	{
		int idx = sqno * 2 + 1;

		// 面数分
		Vector3 up = { 0,1,0 };
		Vector3 startpoint = { pos.x,0,pos.z };
		Plane p = m_planes[idx].GetPlaneInfo().plane;
		Vector3 ans;

		bool sts = LinetoPlaneCross(p, startpoint, up, t, ans);
		if (sts) {
			sts = CheckInTriangle(
				m_planes[idx].GetPlaneInfo().p0,
				m_planes[idx].GetPlaneInfo().p1,
				m_planes[idx].GetPlaneInfo().p2, ans);
			if (sts) {
				oldheight = ans.y;
				return ans.y;
			}
		}
	}

	// 見つからなかった場合は０
	std::cout << "床なし" << "(" << sqno << ")" << std::endl;
	return oldheight;
}

// 平面の方程式を生成する
void Field::MakeEquatation() {
	// 面数を取得
	int numface;
	numface = m_planemesh.GetIndices().size() / 3;

	std::vector<VERTEX_3D> vertices;
	vertices = m_planemesh.GetVertices();

	// 面数分ループ
	for (unsigned int idx = 0; idx < numface; idx++) {
		CPlaneMesh::FACE f = m_planemesh.GetTriangle(idx);

		VERTEX_3D v0 = vertices[f.idx[0]];
		VERTEX_3D v1 = vertices[f.idx[1]];
		VERTEX_3D v2 = vertices[f.idx[2]];

		CPlane p;
		p.MakeEquatation(v0.Position, v1.Position, v2.Position);
		m_planes.emplace_back(p);
	}
}