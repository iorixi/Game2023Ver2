#pragma once
#include "gameObject.h"
#include "enemy.h"

// AnimationModel ƒNƒ‰ƒX‚Ì‘O•ûéŒ¾
class AnimationModel;
class BoundingSphereObj;

namespace Enemy
{
	class HumanObject : public EnemyObject
	{
	private:

		class BoundingSphereObj* enemyHitSphere{};
		class AnimationModel* m_Model;

		float m_MoveTime{};
		float m_BulletTime{};

		int	m_Frame;
		float	m_BlendRate;

		class Shot* m_EnemyShot{};//ËŒ‚

		bool isActive = false;

	public:
		void Init() override;
		void Update() override;
		void PreDraw() override;
		void SetIsActive(bool _isActive);
		bool GetIsActive();

		BoundingSphereObj* GetEnemyHitSphere() { return enemyHitSphere; };
	};
}
