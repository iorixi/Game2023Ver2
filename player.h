#pragma once

#include "gameObject.h"
#include "animationModel.h"
#include <memory>
#include "newModel/CMaterial.h"

//�O���錾
namespace Timer { class DelayCompnent; }
//class Sound::Audio;
namespace Sound { class Audio; }

class BoundingSphereObj;
enum class ActionModo;

namespace Player
{
	class PlayerObject : public GameObject
	{
	private:

		DirectX::SimpleMath::Vector3 m_Velocity{};

		std::shared_ptr<BoundingSphereObj> playerHitSphere{};

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
		class BulletHit* m_BulletHit;//���̓����蔻��

		DirectX::SimpleMath::Vector3 m_HeightToCenter;
		ActionModo actionModo;
		// �}�e���A��
		CMaterial g_material;
		int hp = 100;

	public:
		void Init() override;
		void Update() override;
		void PreDraw() override;
		void Uninit()override;

		void SetIsActive(bool _isActive);
		bool GetIsActive();
		void Damege(int damege);
		int GetHp();

		void SetVelocity(DirectX::SimpleMath::Vector3 Velocity) { m_Velocity = Velocity; }
		DirectX::SimpleMath::Vector3 GetVelocity() { return m_Velocity; }

		class BoundingSphereObj* GetPlayerHitSphere() { return playerHitSphere.get(); };
		ActionModo GetActionModo();
		void SetActionModo(ActionModo actionmodo);
	};
}