#include "CollisionUtils.h"
#include "BoundingSphere.h"

bool IsCollision(BoundingSphereObj& sphere1, BoundingSphereObj& sphere2) {
	// 2�̋��̒��S�Ԃ̋������v�Z
	float distance = (sphere1.GetCenter() - sphere2.GetCenter()).Length();

	// 2�̋��̔��a�̍��v
	float totalRadius = sphere1.GetRadius() + sphere2.GetRadius();

	// �d�Ȃ��Ă��邩�ǂ����𔻒�
	return distance < totalRadius;
}