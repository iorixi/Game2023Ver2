#include "BoundingSphere.h"
#include <algorithm>
#include <vector>

using namespace DirectX::SimpleMath;

Vector3 BoundingSphereObj::CalcWorldPos(Matrix mtx) {
	// ワールド座標を計算する
	return Vector3::Transform(m_Center, mtx);
}

BoundingSphereObj::BoundingSphereObj()
{
}

BoundingSphereObj::BoundingSphereObj(float radius, DirectX::SimpleMath::Vector3 center)
{
	m_Center = center;
	m_Radius = radius;
}

void BoundingSphereObj::Caliculate(const std::vector<VERTEX_3D>& vertices) {
	std::vector<float> lengthlist;

	Vector3 max;
	Vector3 min;
	min = max = vertices[0].Position;

	// AABBの最大と最小を計算する
	for (auto& v : vertices) {
		if (min.x > v.Position.x) min.x = v.Position.x;
		if (min.y > v.Position.y) min.y = v.Position.y;
		if (min.z > v.Position.z) min.z = v.Position.z;

		if (max.x < v.Position.x) max.x = v.Position.x;
		if (max.y < v.Position.y) max.y = v.Position.y;
		if (max.z < v.Position.z) max.z = v.Position.z;
	}

	m_Min = min;
	m_Max = max;

	// 中心座標を計算する
	m_Center = (max + min) * 0.5f;

	for (auto& v : vertices) {
		// 頂点から中心までの距離を計算し、リストに追加する
		float length;
		length = (v.Position - m_Center).Length();
		lengthlist.emplace_back(length);
	}

	// リスト内の最大の長さを半径とする
	auto iter = std::max_element(lengthlist.begin(), lengthlist.end());
	m_Radius = *iter;
}

float BoundingSphereObj::GetRadius() {
	// 半径を取得する
	return m_Radius;
}

Vector3 BoundingSphereObj::GetCenter() {
	// 中心座標を取得する
	return m_Center;
}

BoundingSphereObj BoundingSphereObj::MakeBS(Matrix mtx, Vector3 scale) {
	BoundingSphereObj bs;

	// 中心座標をワールド座標に変換して設定する
	Vector3 wpos = CalcWorldPos(mtx);
	bs.m_Center = wpos;

	// スケールの最大値を求め、半径を再計算する
	float maxsclae = (std::max)((std::max)(scale.x, scale.y), scale.z);
	bs.m_Radius = m_Radius * maxsclae;

	return bs;
}

Matrix BoundingSphereObj::MakeWorldMtx(Vector3 scale, Matrix wmtx) {
	// スケールの最大値を求め、ワールド変換行列を作成する
	float smax;
	smax = (std::max)((std::max)(scale.x, scale.y), scale.z);

	Matrix mtx = Matrix::CreateScale(smax, smax, smax) * wmtx;
	Vector3 bspos = CalcWorldPos(wmtx);

	// バウンディングスフィア用のワールド変換行列にする（中心がずれているので修正）
	mtx._41 = bspos.x;
	mtx._42 = bspos.y;
	mtx._43 = bspos.z;

	return mtx;
}

Vector3 BoundingSphereObj::GetMax() {
	// AABBの最大値を取得する
	return m_Max;
}

Vector3 BoundingSphereObj::GetMin() {
	// AABBの最小値を取得する
	return m_Min;
}