#include "utils.h"

namespace utils
{
void Timer::start()
{
    m_begin = std::chrono::system_clock::now();
}

void Timer::stop()
{
    m_end = std::chrono::system_clock::now();
}

long Timer::duration() const
{
    auto const duration = m_end - m_begin;
    return duration.count();
}

void Log::setFileName(std::string const & fileName)
{
    if (m_enable)
        m_fileName = fileName + ".log";
}

void Log::setFileName(std::string_view fileName)
{
    if (m_enable)
        m_fileName.append(fileName).append(".log");
}

void Log::enableLog(bool enable)
{
    m_enable = enable;
}
}
