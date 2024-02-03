#pragma once
#include "bullet.h"
#include "renderer.h"
#include <memory>

namespace Timer { class ScheduledTask; }

class HomingBullet : public Bullet
{
	//����͒N���ł�������
	enum class CHARACTER ownerChara;

	DirectX::SimpleMath::Vector3 playerPosition;//�v���C���[�̈ʒu

	//�ǔ��e�̑��x
	// �ړ����x��ݒ�i�Ⴆ�΁A���̏ꍇ�̓v���C���[�Ɍ������Ĉ��̑��x�ňړ�����j
	float speed = 0.7f;
	//���̔�ԕ���
	DirectX::SimpleMath::Vector3 directionToEnemy;
	DirectX::SimpleMath::Vector3 directionToPlayer;
	//���̍X�V�O�̕���
	DirectX::SimpleMath::Vector3 oldDirection;

	//�ǔ��e�̒ǔ��ʒu�̍X�V����
	float homingPointUpdateTime = 0.12f;
	//�ǔ��e�̒ǔ��ʒu�̎���
	std::shared_ptr <Timer::ScheduledTask> m_HomingPointUpdateTime;

	//�ǔ����Ă邩
	bool isActive = true;

	//���ƓG�Ƃ̒Ǐ]��؂鋗��
	float closeDistance = 3.0f;
	//���ȏ�̊p�x
	float overAngle = 10.0f;

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
