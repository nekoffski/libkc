#include "Timer.h"

#include <cmath>

#include <fmt/core.h>

namespace kc::prof {

Timer::~Timer() {
    updateValue();
    m_indent--;
    m_idChain = m_idChain.substr(0, m_idChain.size() - m_label.id.size());
}

void Timer::updateValue() {
    static constexpr float weight = 0.5f;

    const auto delta = core::toSeconds(m_clock.now() - m_startTime);

    // hack for perfomance, TODO: measure it actually!
    int hasValue = m_label.value != 0;

    m_label.value  = hasValue * std::lerp(m_label.value, delta, weight) + (1 - hasValue) * delta;
    m_label.indent = m_indent;
}

Timer::Timer(Label& label, int& indent, std::string& idChain, const core::Clock& clock)
    : m_label(label)
    , m_indent(indent)
    , m_idChain(idChain)
    , m_clock(clock)
    , m_startTime(clock.now()) {
    m_indent++;
    m_idChain += label.id;
}

}  // namespace kc::prof
