#pragma once
#include <SimpleMath.h>
#include <cmath>

// ベクトルの角度を計算する関数
float CalculateAngle(const DirectX::SimpleMath::Vector3& v1, const DirectX::SimpleMath::Vector3& v2) {
	// 内積を計算
	float dotProduct = v1.Dot(v2);

	// v1とv2の長さを取得
	float lengthV1 = v1.Length();
	float lengthV2 = v2.Length();

	// 0除算を避ける
	if (lengthV1 == 0.0f || lengthV2 == 0.0f) {
		return 0.0f;
	}

	// 内積の値を角度に変換して返す
	return acos(dotProduct / (lengthV1 * lengthV2));
}