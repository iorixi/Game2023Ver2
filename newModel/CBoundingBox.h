#pragma once
#include	<SimpleMath.h>
#include	"../renderer.h"
#include "../collision.h"

// OBB
class CBoundingBox {
	BoundingBoxOBB	m_obbinfo;			// OBB情報

	// ワールド座標を計算する（対象オブジェクトの行列（RTS）が入った）
	DirectX::SimpleMath::Vector3 CalcWorldPos(DirectX::SimpleMath::Matrix worldmtx);

public:
	// ローカル座標で生成したOBBをワールド空間に変換
	BoundingBoxOBB MakeOBB(
		DirectX::SimpleMath::Matrix worldmtx,			// 対象オブジェクトのワールド変換行列
		DirectX::SimpleMath::Vector3 scale);			// 拡縮率

	// 分離軸に射影した影の重なりをチェックする
	static bool CompareLength(
		const BoundingBoxOBB& ObbA,								// OBB-A
		const BoundingBoxOBB& ObbB,								// OBB-B
		const DirectX::SimpleMath::Vector3& vecSeparate,		// 分離軸
		const DirectX::SimpleMath::Vector3& vecDistance);		// 中心座標を結んだベクトル

	// AABB情報を生成する
	void Caliculate(const std::vector<VERTEX_3D>& vertices);

	// 中心座標を戻す
	DirectX::SimpleMath::Vector3 GetCenter();

	// BBOX描画用のワールド変換行列を生成する
	DirectX::SimpleMath::Matrix MakeWorldMtx(
		DirectX::SimpleMath::Vector3 scale,
		DirectX::SimpleMath::Matrix wmtx);

	// 最大値取得
	DirectX::SimpleMath::Vector3 GetMax();

	// 最小値取得
	DirectX::SimpleMath::Vector3 GetMin();

	// OBB取得
	BoundingBoxOBB GetOBB();
};