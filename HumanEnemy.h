#pragma once
#include "gameObject.h"
#include "enemy.h"
#include <memory>

// AnimationModel ÉNÉâÉXÇÃëOï˚êÈåæ
class AnimationModel;
class BoundingSphereObj;
enum class ActionModo;
namespace Timer { class ScheduledTask; }

namespace Enemy
{
	class HumanObject : public EnemyObject
	{
	private:

		std::shared_ptr<class BoundingSphereObj> enemyHitSphere{};
		std::shared_ptr <Timer::ScheduledTask> m_MoveChangeTask;

		class AnimationModel* m_Model;
		class Move* m_EnemyMove;

		float m_MoveTime{};
		float m_BulletTime{};

		int	m_Frame;
		float	m_BlendRate;

		class Shot* m_EnemyShot{};//éÀåÇ

		bool isActive = false;

		ActionModo actionModo;
		int num;

	public:
		void Init() override;
		void Update() override;
		void PreDraw() override;
		void SetIsActive(bool _isActive);
		bool GetIsActive();

		BoundingSphereObj* GetEnemyHitSphere() { return enemyHitSphere.get(); };

		ActionModo GetActionModo();
		void SetActionModo(ActionModo actionmodo);
	};
}
