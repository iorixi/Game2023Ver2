#pragma once
#include "gameObject.h"

// AnimationModel �N���X�̑O���錾
class AnimationModel;

namespace Enemy
{
	class HumanObject : public GameObject
	{
	private:

		GameObject* m_Child{};
		class AnimationModel* m_Model;

		float m_MoveTime{};
		float m_BulletTime{};

		int	m_Frame;
		float	m_BlendRate;

	public:
		void Init() override;
		void Update() override;
		void PreDraw() override;
	};
}
