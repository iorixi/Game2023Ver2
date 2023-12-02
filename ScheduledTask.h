#pragma once

#include <chrono>

namespace Timer
{
	/// <summary>
	/// 指定した時間ごとにタスクを実行するためのクラス
	/// </summary>
	class ScheduledTask {
		// タスクの実行間隔をミリ秒単位で保持
		std::chrono::milliseconds m_Interval;

		// 次回の実行時刻
		std::chrono::system_clock::time_point m_ScheduledTime;

		// タスクの実行を停止しているかどうかのフラグ
		bool m_Stopped;

		// タスクが停止していた期間
		std::chrono::system_clock::duration m_PausedDuration;

	public:

		// デフォルトコンストラクタ
		ScheduledTask();

		// コンストラクタ：指定した間隔でタスクを実行するScheduledTaskを作成
		ScheduledTask(float interval);

		// タイマーの初期化
		void SetTimer(float interval);

		// 指定した時間が経過したらtrueを返し、同時にフラグをfalseにする
		bool GetFlg();

		// 経過時間を停止
		void Stop();

		// 停止していた経過時間を進める
		void Start();

		// 経過時間を0にリセットして再スタート
		void Restart();

	private:
	};
}