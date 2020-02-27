#include "utils.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace utils {
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

Log::Log(std::string_view fileName)
{
    auto const now = std::chrono::system_clock::now();
    auto const inTimeT = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&inTimeT), "%F %T\n");
    m_outputFile.open(fileName.data() + std::string(".log"), std::ios::app);
    m_outputFile << ss.str();
}

Log::~Log()
{
    if (m_outputFile.is_open())
        m_outputFile.close();
}

} // namespace utils
