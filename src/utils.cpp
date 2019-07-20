#include "utils.h"

namespace utils
{
void Timer::start()
{
    m_start = std::chrono::system_clock::now();
}

void Timer::stop()
{
    m_end = std::chrono::system_clock::now();
}

long Timer::duration() const
{
    auto const duration = m_end - m_start;
    return duration.count();
}
}
