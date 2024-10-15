#include "Profiler.h"

#include <sstream>
#include <iomanip>
#include <algorithm>
#include <ranges>
#include <iostream>

namespace kc::prof {

static std::string spaces(const std::size_t count) {
    static const char space = ' ';
    return std::string(count, space);
}

Profiler::Profiler() : m_currentIndent(-1), m_orderGuard(0) {}

Label& Profiler::prepareLabel(const std::string& name) {
    bool setOrder = not m_labels.contains(name);

    auto& label = m_labels[m_idChain + name];
    label.name  = name;

    if (setOrder) label.order = m_orderGuard++;

    return label;
}

void Profiler::save(
  const std::string& logDestination, const std::string& name,
  const core::FileSystem& fileSystem
) {
    fileSystem.writeFile(
      logDestination + name, format(), core::FileSystem::WritePolicy::override
    );
}

Timer Profiler::createTimer(const std::string& name) {
    return Timer(prepareLabel(name), m_currentIndent, m_idChain, m_clock);
}

void Profiler::pushRegionTimer(const std::string& name) {
    m_regionTimers.emplace(prepareLabel(name), m_currentIndent, m_idChain, m_clock);
}

void Profiler::popRegionTimer() { m_regionTimers.pop(); }

std::string Profiler::format() {
    std::ostringstream stream;

    std::vector<Label> labels;
    labels.reserve(m_labels.size());

    std::ranges::transform(
      m_labels, std::back_inserter(labels),
      [](const auto& record) -> Label { return record.second; }
    );

    std::ranges::sort(labels, [](const Label& lhs, const Label& rhs) -> bool {
        return lhs.order < rhs.order;
    });

    for (const auto& label : labels)
        stream << spaces(label.indent * 2) << '"' << label.name << '"' << " -> "
               << label.value << "s\n";
    return stream.str();
}

}  // namespace kc::prof