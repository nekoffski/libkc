#include "Profiler.h"

#include <algorithm>
#include <fstream>

namespace kc::core {

Profiler::RegionTimer::RegionTimer(float& value, Clock* clock)
    : m_value(value)
    , m_clock(clock)
    , m_startTime(m_clock->now()) {
}

Profiler::RegionTimer::~RegionTimer() {
    updateValue();
}

void Profiler::RegionTimer::updateValue() {
    static constexpr float weight = 0.3f;

    auto deltaTime = m_clock->now() - m_startTime;
    m_value = std::lerp(m_value, toSeconds(deltaTime), weight);
}

Profiler::RegionTimer Profiler::createRegionTimer(const std::string& name) {
    return RegionTimer { m_times[name], &m_clock };
}

void Profiler::saveResults(const std::string& logDestination, const FileSystem& fileSystem) {
    fileSystem.writeFile(logDestination + "logs.perf", formatTimers(), FileSystem::WritePolicy::override);
}

std::string Profiler::formatTimers() {
    static auto compareBySize = [](const auto& lhs, const auto& rhs) {
        return lhs.first.size() < rhs.first.size();
    };

    const auto longestName = std::ranges::max_element(m_times, compareBySize)->first;
    const auto alignment = longestName.size();

    std::ostringstream stream;
    stream << std::fixed;

    for (const auto& [label, value] : m_times)
        stream << std::setw(alignment) << std::left << label << " -> " << value << "s\n";

    return stream.str();
}
}