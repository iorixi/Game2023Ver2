#pragma once
#include "gameObject.h"
#include "enemy.h"
#include <memory>
#include "newModel/CMaterial.h"

class BoundingSphereObj;
enum class ActionModo;
namespace Timer { class ScheduledTask; }

namespace Enemy
{
	class HumanObject : public EnemyObject
	{
	private:

		std::shared_ptr<class BoundingSphereObj> enemyHitSphere{}; // �G�̓����蔻��p�̋��̃I�u�W�F�N�g
		std::shared_ptr<Timer::ScheduledTask> m_MoveChangeTask; // �ړ��؂�ւ��p�̃^�X�N

		class Move* m_EnemyMove; // �G�̈ړ��R���|�[�l���g

		float m_MoveTime{}; // �ړ�����
		float m_BulletTime{}; // �e�̔��ˎ���

		int m_Frame; // �t���[����
		float m_BlendRate; // �u�����h��

		class Shot* m_EnemyShot{}; // �G�̎ˌ��R���|�[�l���g

		bool isActive = false; // �A�N�e�B�u���

		ActionModo actionModo;// �A�N�V�������[�h
		int actionModoSelect; // �A�N�V�������[�h�̑I��

		// �}�e���A��
		CMaterial g_material;

	public:
		// �������֐�
		void Init() override;
		// �I�������֐�
		void Uninit() override;
		// �X�V�֐�
		void Update() override;
		// �`��O�����֐�
		void PreDraw() override;
		// �A�N�e�B�u��Ԃ�ݒ肷��֐�
		void SetIsActive(bool _isActive);
		// �A�N�e�B�u��Ԃ��擾����֐�
		bool GetIsActive();

		// �G�̓����蔻��p�̋��̃I�u�W�F�N�g���擾����֐�
		BoundingSphereObj* GetEnemyHitSphere() { return enemyHitSphere.get(); };

		// �A�N�V�������[�h���擾����֐�
		ActionModo GetActionModo();
		// �A�N�V�������[�h��ݒ肷��֐�
		void SetActionModo(ActionModo actionmodo);
	};
}
