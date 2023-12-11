#pragma once
#include "gameObject.h"

// AnimationModel �N���X�̑O���錾
class AnimationModel;
class BoundingSphere;

namespace Enemy
{
	class HumanObject : public GameObject
	{
	private:

		BoundingSphere* enemyHitSphere{};
		class AnimationModel* m_Model;

		float m_MoveTime{};
		float m_BulletTime{};

		int	m_Frame;
		float	m_BlendRate;

	public:
		void Init() override;
		void Update() override;
		void PreDraw() override;

		BoundingSphere* GetEnemyHitSphere() { return enemyHitSphere; };
	};
}
