#pragma once

#include "gameObject.h"
#include <vector>

class BoundingSphere : public GameObject
{
private:
	DirectX::SimpleMath::Vector3 m_Center;  // スフィアの中心座標
	float m_Radius;  // スフィアの半径
	DirectX::SimpleMath::Vector3 m_Max;  // AABB（軸に平行な境界ボックス）の最大値
	DirectX::SimpleMath::Vector3 m_Min;  // AABBの最小値

	// ワールド座標系での位置を計算する関数
	DirectX::SimpleMath::Vector3 CalcWorldPos(DirectX::SimpleMath::Matrix mtx);

public:
	BoundingSphere();
	BoundingSphere(float m_Radius, DirectX::SimpleMath::Vector3 m_Center);

	// 与えられた頂点からバウンディングスフィアを計算する関数
	void Caliculate(const std::vector<VERTEX_3D>& vertices);

	// スフィアの半径を取得する関数
	float GetRadius();

	// スフィアの中心座標を取得する関数
	DirectX::SimpleMath::Vector3 GetCenter();

	// スフィアをワールド座標系でスケーリングする関数
	BoundingSphere MakeBS(DirectX::SimpleMath::Matrix mtx, DirectX::SimpleMath::Vector3 scale);

	// スケーリングとワールド座標変換行列から新しいワールド変換行列を作成する関数
	DirectX::SimpleMath::Matrix MakeWorldMtx(DirectX::SimpleMath::Vector3 scale, DirectX::SimpleMath::Matrix wmtx);

	// AABBの最大値を取得する関数
	DirectX::SimpleMath::Vector3 GetMax();

	// AABBの最小値を取得する関数
	DirectX::SimpleMath::Vector3 GetMin();

	void SetCenter(DirectX::SimpleMath::Vector3 pos) { m_Center = pos; }
	void SetRadius(float radius) { m_Radius = radius; }
};
