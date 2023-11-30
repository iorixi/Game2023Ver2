#include "ScheduledTask .h"
#include <thread>

ScheduledTask::ScheduledTask(std::chrono::seconds interval) : m_interval(interval) {
    // �ŏ��̎��s���Ԃ�ݒ�
    m_scheduledTime = std::chrono::system_clock::now() + m_interval;
}

bool ScheduledTask::GetScheduledTask() {
    // ���݂̎��Ԃ��擾
    auto currentTime = std::chrono::system_clock::now();

    // �w�肵�����ԂɂȂ�܂őҋ@
    while (currentTime < m_scheduledTime) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        currentTime = std::chrono::system_clock::now();
    }

    // �t���O��false�ɂ��āA����̎��s���Ԃ�ݒ�
    m_scheduledTime = std::chrono::system_clock::now() + m_interval;

    return true;
}
