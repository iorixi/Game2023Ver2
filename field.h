#pragma once
#include	<d3d11.h>
#include	<SimpleMath.h>
#include	"gameObject.h"
#include	"CUndulationPlaneMesh.h"
#include	"CMeshRenderer.h"

class Field : public GameObject
{

private:

	CMeshRenderer		m_renderer;						// フィールドのレンダラ

	ID3D11ShaderResourceView* m_Texture{};

	// 平面プレーン
	CUndulationPlaneMesh	m_planemesh;				// 平面メッシュ

	std::vector<CPlane> m_planes;						// 平面の方程式群

	// 平面の方程式を生成する（全面）
	void MakeEquatation();

public:
	// 指定した場所の高さを取得
	float GetFieldHeight(DirectX::SimpleMath::Vector3 pos);
	float GetFieldHeightBySqno(DirectX::SimpleMath::Vector3 pos);

	// GetMAX XZ
	DirectX::SimpleMath::Vector3 GetMax() {

		DirectX::SimpleMath::Vector3 max;

		max.x = m_planemesh.GetWidth() / 2.0f;
		max.z = m_planemesh.GetHeight() / 2.0f;
		max.y = 0.0f;

		return max;
	}

	// GetMIN XZ
	DirectX::SimpleMath::Vector3 GetMin() {
		DirectX::SimpleMath::Vector3 min;

		min.x = -m_planemesh.GetWidth() / 2.0f;
		min.z = -m_planemesh.GetHeight() / 2.0f;
		min.y = 0.0f;

		return min;
	}

	void Init();
	void Uninit();
	void Update();
	void Draw();
};