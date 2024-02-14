#include	"CBoundingSphere.h"
using namespace DirectX::SimpleMath;
#undef max//衝突回避

// ワールド座標を計算する（対象オブジェクトの行列（RTS）が入った）
Vector3 CBoundingSphere::CalcWorldPos(DirectX::SimpleMath::Matrix mtx) {
	return Vector3::Transform(m_center, mtx);
}

// BS情報を生成する
void CBoundingSphere::Caliculate(const std::vector<VERTEX_3D>& vertices) {
	std::vector<float > lengthlist;

	DirectX::SimpleMath::Vector3 max;
	DirectX::SimpleMath::Vector3 min;

	// 最大値と最小値を求める
	min = max = vertices[0].Position;
	for (auto& v : vertices) {
		if (min.x > v.Position.x) min.x = v.Position.x;
		if (min.y > v.Position.y) min.y = v.Position.y;
		if (min.z > v.Position.z) min.z = v.Position.z;

		if (max.x < v.Position.x) max.x = v.Position.x;
		if (max.y < v.Position.y) max.y = v.Position.y;
		if (max.z < v.Position.z) max.z = v.Position.z;
	}

	m_min = min;
	m_max = max;

	std::cout << m_min.x << "," << m_min.y << "," << m_min.z << std::endl;
	std::cout << m_max.x << "," << m_max.y << "," << m_max.z << std::endl;

	// 中心を求める
	m_center = (max + min) * 0.5f;

	// 半径を求める
	for (auto& v : vertices) {
		float length;
		length = (v.Position - m_center).Length();
		lengthlist.emplace_back(length);
	}

	// 最大の長さを求める
	auto iter = std::max_element(lengthlist.begin(), lengthlist.end());
	m_radius = *iter;
}

// 半径を戻す
float CBoundingSphere::GetRadius() {
	return m_radius;
}

// 中心座標を戻す
Vector3 CBoundingSphere::GetCenter() {
	return m_center;
}

// BS情報生成
BoundingSphere CBoundingSphere::MakeBS(
	DirectX::SimpleMath::Matrix mtx,			// オブジェクトの行列（RTS情報）
	DirectX::SimpleMath::Vector3 scale)			// オブジェクトのスケール値
{
	BoundingSphere bs;

	// 中心座標計算
	Vector3 wpos = CalcWorldPos(mtx);
	bs.center = wpos;

	// 半径の再計算（拡縮がある為）
	float maxsclae = std::max(std::max(scale.x, scale.y), scale.z);
	bs.radius = m_radius * maxsclae;

	return bs;
}

// BS描画用のワールド変換行列を生成する
Matrix CBoundingSphere::MakeWorldMtx(
	Vector3 scale,
	Matrix wmtx) {
	// 単純に３Ｄジオメトリ情報からＢＳ情報を作成してる
	//（拡縮が考慮されていない）
	float smax;
	smax = std::max(std::max(scale.x, scale.y), scale.z);

	// 最大の拡大率で行列生成
	Matrix mtx = Matrix::CreateScale(smax, smax, smax) * wmtx;
	Vector3 bspos = CalcWorldPos(wmtx);

	// バウンディングスフィア用のワールド変換行列にする（中心がずれてるので修正）
	mtx._41 = bspos.x;
	mtx._42 = bspos.y;
	mtx._43 = bspos.z;

	return mtx;
}

// 最大値取得
Vector3 CBoundingSphere::GetMax() {
	return m_max;
}

// 最小値取得
Vector3 CBoundingSphere::GetMin() {
	return m_min;
}