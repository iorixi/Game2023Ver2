#pragma once

#include "gameObject.h"
#include "animationModel.h"

//前方宣言
namespace Timer { class DelayCompnent; }
//class Sound::Audio;
namespace Sound { class Audio; }

namespace Player
{
	class PlayerObject : public GameObject
	{
	private:

		DirectX::SimpleMath::Vector3 m_Velocity{};

		class AnimationModel* m_Model;
		int	m_Frame;
		float	m_BlendRate;

		Timer::DelayCompnent* m_Delay{};
		Sound::Audio* m_SE{};

		//プレイヤー
		class Move* m_PlayerMove{};				//移動
		class Floating* m_PlayerFloating{};		//浮遊
		class Evasive* m_PlayerEvasive{};		//回避
		class Shot* m_PlayerShot{};//射撃

		DirectX::SimpleMath::Vector3 m_HeightToCenter;

	public:
		void Init() override;
		void Update() override;
		void PreDraw() override;

		void SetVelocity(DirectX::SimpleMath::Vector3 Velocity) { m_Velocity = Velocity; }
		DirectX::SimpleMath::Vector3 GetVelocity() { return m_Velocity; }
	};
}