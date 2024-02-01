#pragma once

#include "gameObject.h"
#include "animationModel.h"

//�O���錾
namespace Timer { class DelayCompnent; }
//class Sound::Audio;
namespace Sound { class Audio; }

class BoundingSphereObj;

namespace Player
{
	class PlayerObject : public GameObject
	{
	private:

		DirectX::SimpleMath::Vector3 m_Velocity{};

		class BoundingSphereObj* playerHitSphere{};

		class AnimationModel* m_Model;
		int	m_Frame;
		float	m_BlendRate;

		Timer::DelayCompnent* m_Delay{};
		Sound::Audio* m_SE{};

		//�A�N�e�B�u��Ԃ�
		bool isActive = false;

		//�v���C���[
		class Move* m_PlayerMove{};				//�ړ�
		class Floating* m_PlayerFloating{};		//���V
		class Evasive* m_PlayerEvasive{};		//���
		class Shot* m_PlayerShot{};//�ˌ�

		DirectX::SimpleMath::Vector3 m_HeightToCenter;

	public:
		void Init() override;
		void Update() override;
		void PreDraw() override;

		void SetIsActive(bool _isActive);
		bool GetIsActive();

		void SetVelocity(DirectX::SimpleMath::Vector3 Velocity) { m_Velocity = Velocity; }
		DirectX::SimpleMath::Vector3 GetVelocity() { return m_Velocity; }

		class BoundingSphereObj* GetPlayerHitSphere() { return playerHitSphere; };
	};
}