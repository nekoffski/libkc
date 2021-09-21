#include "Clock.h"

#include <ctime>
#include <iomanip>

namespace kc::core {

float toSeconds(const Clock::TimePoint& timePoint) {
    using namespace std::chrono;
    return duration_cast<microseconds>(timePoint.time_since_epoch()).count() / Clock::microsecondsInSecond;
}

std::string Clock::getTimeString(const std::string& format) const {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, format.c_str());
    return oss.str();
}

Clock::TimePoint Clock::now() const {
    return m_clock.now();
}

float Clock::nowAsFloat() const {
    return toSeconds(now());
}

}