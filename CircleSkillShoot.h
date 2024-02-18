#pragma once
#include "bullet.h"
#include "renderer.h"
#include <memory>

namespace Timer { class ScheduledTask; }

class CircleSkillShoot : public Bullet
{
	std::unique_ptr <Timer::ScheduledTask> m_ShootTime;
	//����͒N���ł�������
	enum class CHARACTER ownerChara;

	bool m_ShootMoveFlg = false;//���������Ă��邩

	DirectX::SimpleMath::Vector3 m_EnemyPosition;
	bool m_EnemyPositionSaved = false;//�Z�[�u���Ă邩

	DirectX::SimpleMath::Vector3 m_PositionStart;
	float Degree = 0.0f;

	// �ʂ̑��x��ݒ�
	float speed = 0.7f; // ���̑��x�l

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
