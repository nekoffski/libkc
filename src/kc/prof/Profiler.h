#pragma once

#include <string>
#include <map>
#include <stack>

#include "kc/core/Macros.h"
#include "kc/core/Clock.h"
#include "kc/core/Singleton.hpp"
#include "kc/core/FileSystem.h"

#include "Label.h"
#include "Timer.h"

namespace kc::prof {

class Profiler : public core::Singleton<Profiler> {
   public:
    explicit Profiler();

    Timer createTimer(const std::string& name);
    void pushRegionTimer(const std::string& name);
    void popRegionTimer();

    std::string format();

    void save(
        const std::string& name = "logs.perf", const std::string& logDestination = "./",
        const core::FileSystem& fileSystem = core::FileSystem{}
    );

   private:
    Label& prepareLabel(const std::string& name);

    core::Clock m_clock;
    int m_currentIndent;
    std::string m_idChain = core::generateUuid();
    std::unordered_map<std::string, Label> m_labels;
    std::stack<Timer> m_regionTimers;

    int m_orderGuard;
};

}  // namespace kc::prof

// clang-format off
#ifdef KC_DISABLE_PROFILER
    #define PROFILE_SCOPE(name)
    #define PROFILE_FUNCTION
#else
    #define PROFILE_SCOPE(name) const auto ANONYMOUS_VAR(name)              \
        = kc::prof::Profiler::get().createTimer(#name); 

    #define PROFILE_FUNCTION() const auto ANONYMOUS_VAR(__PRETTY_FUNCTION__) \
        = kc::prof::Profiler::get().createTimer(__PRETTY_FUNCTION__);

    #define PROFILE_REGION_BEGIN(name) kc::prof::Profiler::get().pushRegionTimer(#name);
    #define PROFILE_REGION_END() kc::prof::Profiler::get().popRegionTimer();
#endif
// clang-format on