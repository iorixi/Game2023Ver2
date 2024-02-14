#include	"CBoundingBox.h"
using namespace DirectX::SimpleMath;

// ワールド座標を計算する（対象オブジェクトの行列（RTS）が入った）
Vector3 CBoundingBox::CalcWorldPos(Matrix worldmtx) {
	return Vector3::Transform(m_obbinfo.center, worldmtx);
}

// ローカル座標で生成したOBBをワールド空間に変換
BoundingBoxOBB CBoundingBox::MakeOBB(
	Matrix worldmtx,			// 対象オブジェクトのワールド変換行列
	Vector3 scale) {			// 拡縮率
	// 出力用
	BoundingBoxOBB obb;
	obb = m_obbinfo;

	// 平行移動
	obb.worldcenter = Vector3::Transform(m_obbinfo.center, worldmtx);

	// 移動成分カット
	worldmtx._41 = 0.0f;
	worldmtx._42 = 0.0f;
	worldmtx._43 = 0.0f;

	// 軸回転
	obb.axisX = DirectX::SimpleMath::Vector3::Transform(m_obbinfo.axisX, worldmtx);
	obb.axisY = DirectX::SimpleMath::Vector3::Transform(m_obbinfo.axisY, worldmtx);
	obb.axisZ = DirectX::SimpleMath::Vector3::Transform(m_obbinfo.axisZ, worldmtx);

	// 軸を正規化
	obb.axisX.Normalize();
	obb.axisY.Normalize();
	obb.axisZ.Normalize();

	// 拡縮を考慮
	obb.lengthx *= scale.x;
	obb.lengthy *= scale.y;
	obb.lengthz *= scale.z;

	return obb;
}

// AABB情報を生成する
void CBoundingBox::Caliculate(const std::vector<VERTEX_3D>& vertices) {
	Vector3 max;
	Vector3 min;

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

	m_obbinfo.min = min;
	m_obbinfo.max = max;

	// 中心を求める
	m_obbinfo.worldcenter = m_obbinfo.center = (max + min) * 0.5f;

	// 軸を設定
	m_obbinfo.axisX = Vector3(1, 0, 0);
	m_obbinfo.axisY = Vector3(0, 1, 0);
	m_obbinfo.axisZ = Vector3(0, 0, 1);

	// 長さを設定
	DirectX::SimpleMath::Vector3 len;
	len = m_obbinfo.max - m_obbinfo.min;

	m_obbinfo.lengthx = fabs(len.x);
	m_obbinfo.lengthy = fabs(len.y);
	m_obbinfo.lengthz = fabs(len.z);
}

// 中心座標を戻す
Vector3 CBoundingBox::GetCenter() {
	return m_obbinfo.center;
}

// BBOX描画用のワールド変換行列を生成する
Matrix CBoundingBox::MakeWorldMtx(
	Vector3 scale,
	Matrix wmtx) {
	// 拡縮を考慮
	Matrix mtx = wmtx;

	// 中心座標を再計算
	Vector3 position = CalcWorldPos(wmtx);

	// バウンディングスフィア用のワールド変換行列にする（中心がずれてるので修正）
	mtx._41 = position.x;
	mtx._42 = position.y;
	mtx._43 = position.z;

	return mtx;
}

// 最大値取得
Vector3 CBoundingBox::GetMax() {
	return m_obbinfo.max;
}

// 最小値取得
Vector3 CBoundingBox::GetMin() {
	return m_obbinfo.min;
}

// OBB取得
BoundingBoxOBB CBoundingBox::GetOBB() {
	return m_obbinfo;
}

// OBBの重なりを判定する
bool CBoundingBox::CompareLength(
	const BoundingBoxOBB& ObbA,		// OBB-A
	const BoundingBoxOBB& ObbB,		// OBB-B
	const Vector3& vecSeparate,		// 分離軸
	const Vector3& vecDistance)		// 中心座標を結んだベクトル
{
	float fDistance{};

	// 分離軸に射影した中心間を結ぶベクトル
	fDistance = vecDistance.Dot(vecSeparate);	// 内積を計算
	fDistance = fabsf(fDistance);				// プラスにする

	//分離軸上にボックスAを射影した影の長さ
	float fShadowA = 0;

	//分離軸上にボックスＢを射影した影の長さ
	float fShadowB = 0;

	//ボックスAの”影”を算出
	float fShadowAx{};
	float fShadowAy{};
	float fShadowAz{};

	// X軸を分離軸に射影
	fShadowAx = vecSeparate.Dot(ObbA.axisX);
	fShadowAx = fabsf(fShadowAx * (ObbA.lengthx / 2.0f));

	// Y軸を分離軸に射影
	fShadowAy = vecSeparate.Dot(ObbA.axisY);
	fShadowAy = fabsf(fShadowAy * (ObbA.lengthy / 2.0f));

	// Z軸を分離軸に射影
	fShadowAz = vecSeparate.Dot(ObbA.axisZ);
	fShadowAz = fabsf(fShadowAz * (ObbA.lengthz / 2.0f));

	// 分離軸に射影した影の長さを求める
	fShadowA = fShadowAx + fShadowAy + fShadowAz;

	//ボックスBの”影”を算出
	float fShadowBx{};
	float fShadowBy{};
	float fShadowBz{};

	// X軸を分離軸に射影
	fShadowBx = vecSeparate.Dot(ObbB.axisX);
	fShadowBx = fabsf(fShadowBx * (ObbB.lengthx / 2.0f));

	// Y軸を分離軸に射影
	fShadowBy = vecSeparate.Dot(ObbB.axisY);
	fShadowBy = fabsf(fShadowBy * (ObbB.lengthy / 2.0f));

	// Z軸を分離軸に射影
	fShadowBz = vecSeparate.Dot(ObbB.axisZ);
	fShadowBz = fabsf(fShadowBz * (ObbB.lengthz / 2.0f));

	// 分離軸に射影した影の長さを求める
	fShadowB = fShadowBx + fShadowBy + fShadowBz;

	if (fDistance > fShadowA + fShadowB) {
		return false;
	}

	return true;
}