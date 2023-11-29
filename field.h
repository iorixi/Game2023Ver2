#pragma once
#include	<d3d11.h>
#include	<SimpleMath.h>
#include	"gameObject.h"
#include	"CUndulationPlaneMesh.h"
#include	"CMeshRenderer.h"

class Field : public GameObject
{

private:

	CMeshRenderer		m_renderer;						// �t�B�[���h�̃����_��

	ID3D11ShaderResourceView* m_Texture{};

	// ���ʃv���[��
	CUndulationPlaneMesh	m_planemesh;				// ���ʃ��b�V��

	std::vector<CPlane> m_planes;						// ���ʂ̕������Q

	// ���ʂ̕������𐶐�����i�S�ʁj
	void MakeEquatation();

public:
	// �w�肵���ꏊ�̍������擾
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