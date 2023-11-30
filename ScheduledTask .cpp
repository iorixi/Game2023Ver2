#include "ScheduledTask .h"
#include <thread>

ScheduledTask::ScheduledTask(std::chrono::seconds interval) : m_interval(interval) {
    // 最初の実行時間を設定
    m_scheduledTime = std::chrono::system_clock::now() + m_interval;
}

bool ScheduledTask::GetScheduledTask() {
    // 現在の時間を取得
    auto currentTime = std::chrono::system_clock::now();

    // 指定した時間になるまで待機
    while (currentTime < m_scheduledTime) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        currentTime = std::chrono::system_clock::now();
    }

    // フラグをfalseにして、次回の実行時間を設定
    m_scheduledTime = std::chrono::system_clock::now() + m_interval;

    return true;
}
