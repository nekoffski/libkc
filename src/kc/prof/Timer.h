#pragma once

#include "fwd.h"
#include "Label.h"

#include "kc/core/Clock.h"

namespace kc::prof {

class Timer {
    friend class Profiler;

   public:
    explicit Timer(Label& label, int& indent, std::string& idChain, const core::Clock& clock);
    ~Timer();

    Timer(Timer&&)      = delete;
    Timer(const Timer&) = delete;

    Timer& operator=(const Timer&) = delete;
    Timer& operator=(Timer&&)      = delete;

   private:
    void updateValue();

    Label& m_label;
    int& m_indent;
    std::string& m_idChain;
    const core::Clock& m_clock;

    core::Clock::TimePoint m_startTime;
};

}  // namespace kc::prof
