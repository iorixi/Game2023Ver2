#pragma once

#include <chrono>

class ScheduledTask {
    std::chrono::seconds m_interval;
    std::chrono::system_clock::time_point m_scheduledTime;
public:
    ScheduledTask(std::chrono::seconds interval);

    // �w�肵�����Ԃ��o�߂�����true��Ԃ��A�����Ƀt���O��false�ɂ���
    bool GetScheduledTask();

private:

};
