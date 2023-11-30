#pragma once

#include <chrono>

class ScheduledTask {
    std::chrono::seconds m_interval;
    std::chrono::system_clock::time_point m_scheduledTime;
public:
    ScheduledTask(std::chrono::seconds interval);

    // 指定した時間が経過したらtrueを返し、同時にフラグをfalseにする
    bool GetScheduledTask();

private:

};
