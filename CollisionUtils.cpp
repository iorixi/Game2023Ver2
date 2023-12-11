#include "CollisionUtils.h"
#include "BoundingSphere.h"

bool IsCollision(BoundingSphere& sphere1, BoundingSphere& sphere2) {
	// 2つの球の中心間の距離を計算
	float distance = (sphere1.GetCenter() - sphere2.GetCenter()).Length();

	// 2つの球の半径の合計
	float totalRadius = sphere1.GetRadius() + sphere2.GetRadius();

	// 重なっているかどうかを判定
	return distance < totalRadius;
}