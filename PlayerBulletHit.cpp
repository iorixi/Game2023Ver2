#include "PlayerBullerHit.h"
#include <vector>
#include "scene.h"
#include "HumanEnemy.h"
#include "HomingBullet.h"
#include "manager.h"
#include "renderer.h"
#include "CharaEnum.h"
#include "collision.h"
#include "cylinder.h"
#include "CollisionUtils.h"

using namespace DirectX::SimpleMath;

void Player::BulletHit::Update()
{
	//���̓����蔻�菈��
	HomingHit();
}

void Player::BulletHit::HomingHit()
{
	hitFlg = false;
	//���݂̃V�[�����擾
	Scene* scene = Manager::GetScene();

	//���݂̃V�[���̓G�̃I�u�W�F�N�g���擾
	Enemy::HumanObject* enemy = scene->GetGameObject<Enemy::HumanObject>();

	std::vector<Enemy::HumanObject*> enemyList = scene->GetGameObjects<Enemy::HumanObject>();
	std::vector<HomingBullet*> bulletList = scene->GetGameObjects<HomingBullet>();

	//�G�ւ̓����蔻��
	for (Enemy::HumanObject* enemy : enemyList)
	{
		//���ւ̓����蔻��
		for (HomingBullet* bullet : bulletList)
		{
			Vector3 enemyPosition = enemy->GetPosition();
			BoundingSphereObj* enemyHitSphere = enemy->GetEnemyHitSphere();
			BoundingSphereObj* bulletHitSphere = bullet->GetBulletHitSphere();

			//�I�[�i�[���v���C���[�Ȃ瓖���蔻��
			if (bullet->GetBulletOwner() == CHARACTER::PLAYER)
			{
				//���̓����蔻��
				if (IsCollision(*enemyHitSphere, *bulletHitSphere))
				{
					//score->AddCount(1);
					//Sound::Audio* m_SE = player->GetComponent<Sound::Audio>();
					//m_SE->Play();
					hitFlg = true;

					enemy->Damege(1);
					bullet->SetDestroy();
				}
			}
		}
	}
}

bool Player::BulletHit::GetHitFlg()
{
	return hitFlg;
}