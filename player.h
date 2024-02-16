#pragma once

#include "gameObject.h"
#include "animationModel.h"
#include <memory>
#include "newModel/CMaterial.h"

//前方宣言
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

		//アクティブ状態か
		bool isActive = false;

		//プレイヤー
		class Move* m_PlayerMove{};				//移動
		class Floating* m_PlayerFloating{};		//浮遊
		class Evasive* m_PlayerEvasive{};		//回避
		class Shot* m_PlayerShot{};//射撃
		class BulletHit* m_BulletHit;//球の当たり判定

		DirectX::SimpleMath::Vector3 m_HeightToCenter;
		ActionModo actionModo;
		// マテリアル
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