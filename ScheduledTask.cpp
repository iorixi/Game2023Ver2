#include "ScheduledTask.h"
#include <thread>
#include <iostream>

using namespace Timer;

// デフォルトコンストラクタ
ScheduledTask::ScheduledTask() {
}

// コンストラクタの実装
ScheduledTask::ScheduledTask(float interval) : m_Interval(static_cast<long long>(interval * 1000.0)), m_Stopped(false) {
	// 最初の実行時間を設定
	m_ScheduledTime = std::chrono::system_clock::now() + m_Interval;
}

// タイマーの設定
void ScheduledTask::SetTimer(float interval) {
	// intervalをミリ秒に変換してタイマーの設定
	m_Interval = std::chrono::milliseconds(static_cast<long long>(interval * 1000.0));
	m_ScheduledTime = std::chrono::system_clock::now() + m_Interval;
}

// 指定した時間が経過したらtrueを返し、同時にフラグをfalseにする
bool ScheduledTask::GetFlg() {
	if (m_Stopped) {
		return false;  // 経過時間が停止中は常にfalseを返す
	}

	// 現在の時間を取得
	auto currentTime = std::chrono::system_clock::now();

	// 指定した時間になるまで待機
	while (currentTime < m_ScheduledTime) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));  // 小さなウェイト
		currentTime = std::chrono::system_clock::now();
	}

	// フラグをfalseにして、次回の実行時間を設定
	m_ScheduledTime = std::chrono::system_clock::now() + m_Interval;

	return true;
}

// 経過時間を停止
void ScheduledTask::Stop() {
	if (!m_Stopped) {
		m_PausedDuration = std::chrono::system_clock::now() - m_ScheduledTime;
		m_Stopped = true;
	}
}

// 停止していた経過時間を進める
void ScheduledTask::Start() {
	if (m_Stopped) {
		m_ScheduledTime = std::chrono::system_clock::now() + m_PausedDuration;
		m_Stopped = false;
	}
}

// 経過時間を0にリセットして再スタート
void ScheduledTask::Restart() {
	m_ScheduledTime = std::chrono::system_clock::now() + m_Interval;
	m_Stopped = false;
}

/*
  0.8秒ごとにタスクを実行するScheduledTaskを作成
  ScheduledTask scheduledTask(0.8f);

  // Getメソッドを呼ぶことで指定した時間が経過するまで待機し、その後にtrueを取得
  bool result = scheduledTask.GetFlg();

  if (result) {
	  // 指定した時間が経過した時の処理をここに書く
  }
  else {
	  // エラー処理等をここに書く
  }

  // 経過時間を停止
  scheduledTask.Stop();

  // 経過時間を進める
  scheduledTask.Start();
*/