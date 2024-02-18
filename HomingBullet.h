#pragma once
#include "bullet.h"
#include "renderer.h"
#include <memory>

namespace Timer { class ScheduledTask; }
class Shader;

class HomingBullet : public Bullet
{
	//これは誰が打った球か
	enum class CHARACTER ownerChara;

	DirectX::SimpleMath::Vector3 playerPosition;//プレイヤーの位置

	//追尾弾の速度
	// 移動速度を設定（例えば、この場合はプレイヤーに向かって一定の速度で移動する）
	float speed = 0.7f;
	//球の飛ぶ方向
	DirectX::SimpleMath::Vector3 directionToEnemy;
	DirectX::SimpleMath::Vector3 directionToPlayer;
	DirectX::SimpleMath::Vector3 m_PositionStart;
	//球の更新前の方向
	DirectX::SimpleMath::Vector3 oldDirection;

	//追尾弾の追尾位置の更新時間
	float homingPointUpdateTime = 0.12f;
	//追尾弾の追尾位置の時間
	std::shared_ptr <Timer::ScheduledTask> m_HomingPointUpdateTime;

	//追尾してるか
	bool isActive = true;

	//球と敵との追従を切る距離
	float closeDistance = 3.0f;
	//一定以上の角度
	float overAngle = 10.0f;

	//一定以下の速度に足していると削除する変数
	float minVelocity = 0.001f;;

	//球の消える時間
	std::shared_ptr <Timer::ScheduledTask> m_HomingTimeDestroy;
	bool m_timerSet = false;
	float m_timer = 3.0f;

public:
	HomingBullet();
	HomingBullet(enum class CHARACTER chara);
	~HomingBullet();

	static void Load();
	static void Unload();

	void Init() override;
	void Update() override;
	void SetBulletOwner(enum class CHARACTER chara);
	enum class CHARACTER GetBulletOwner();

private:
};
