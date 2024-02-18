#pragma once
#include "bullet.h"
#include "renderer.h"
#include <memory>

namespace Timer { class ScheduledTask; }

class CircleSkillShoot : public Bullet
{
	std::unique_ptr <Timer::ScheduledTask> m_ShootTime;
	//これは誰が打った球か
	enum class CHARACTER ownerChara;

	bool m_ShootMoveFlg = false;//球が動いているか

	DirectX::SimpleMath::Vector3 m_EnemyPosition;
	bool m_EnemyPositionSaved = false;//セーブしてるか

	DirectX::SimpleMath::Vector3 m_PositionStart;
	float Degree = 0.0f;

	// 玉の速度を設定
	float speed = 0.7f; // 仮の速度値

public:
	CircleSkillShoot();
	CircleSkillShoot(enum class CHARACTER chara);

	~CircleSkillShoot();

	static void Load();
	static void Unload();

	void Init() override;
	void Update() override;

	void SetBulletOwner(enum class CHARACTER chara);
	void SetBulletDegree(float degree);
	enum class CHARACTER GetBulletOwner();
private:
};
