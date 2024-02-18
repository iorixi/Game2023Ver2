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

		std::shared_ptr<class BoundingSphereObj> enemyHitSphere{}; // 敵の当たり判定用の球体オブジェクト
		std::shared_ptr<Timer::ScheduledTask> m_MoveChangeTask; // 移動切り替え用のタスク

		class Move* m_EnemyMove; // 敵の移動コンポーネント

		float m_MoveTime{}; // 移動時間
		float m_BulletTime{}; // 弾の発射時間

		int m_Frame; // フレーム数
		float m_BlendRate; // ブレンド率

		class Shot* m_EnemyShot{}; // 敵の射撃コンポーネント

		bool isActive = false; // アクティブ状態

		ActionModo actionModo;// アクションモード
		int actionModoSelect; // アクションモードの選択

		// マテリアル
		CMaterial g_material;

	public:
		// 初期化関数
		void Init() override;
		// 終了処理関数
		void Uninit() override;
		// 更新関数
		void Update() override;
		// 描画前処理関数
		void PreDraw() override;
		// アクティブ状態を設定する関数
		void SetIsActive(bool _isActive);
		// アクティブ状態を取得する関数
		bool GetIsActive();

		// 敵の当たり判定用の球体オブジェクトを取得する関数
		BoundingSphereObj* GetEnemyHitSphere() { return enemyHitSphere.get(); };

		// アクションモードを取得する関数
		ActionModo GetActionModo();
		// アクションモードを設定する関数
		void SetActionModo(ActionModo actionmodo);
	};
}
