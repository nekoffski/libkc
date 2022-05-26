#pragma once

#include <chrono>
#include <string>

namespace kc::core {

class Clock {
   public:
    using ClockType = std::chrono::steady_clock;
    using TimePoint = std::chrono::time_point<ClockType>;

    inline static constexpr float microsecondsInSecond = 1000 * 1000;

    std::string getTimeString(const std::string& format) const;

    TimePoint now() const;

    float nowAsFloat() const;

   private:
    ClockType m_clock;
};

template <typename U, typename T> float toSeconds(const std::chrono::duration<U, T>& duration) {
    using namespace std::chrono;
    return duration_cast<microseconds>(duration).count() / Clock::microsecondsInSecond;
}

float toSeconds(const Clock::TimePoint& timePoint);

}  // namespace kc::core