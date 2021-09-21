#pragma once

#include <memory>
#include <unordered_map>

#include "Clock.h"
#include "FileSystem.h"
#include "Macros.h"
#include "Singleton.hpp"

namespace kc::core {

class Profiler : public Singleton<Profiler> {
    class RegionTimer {
    public:
        explicit RegionTimer(float& value, Clock* clock);
        ~RegionTimer();

    private:
        void updateValue();

        float& m_value;

        Clock* m_clock;
        Clock::TimePoint m_startTime;
    };

public:
    explicit Profiler(const Clock& clock = Clock {});

    RegionTimer createRegionTimer(const std::string& name);

    void saveResults(const std::string& logDestination = "./", const FileSystem& fileSystem = FileSystem {});

private:
    std::string formatTimers();

    Clock m_clock;
    std::unordered_map<std::string, float> m_times;
};

}

// clang-format off
#ifdef KC_DISABLE_PROFILER
    #define LOG_PROFILE_REGION(name)
    #define LOG_PROFILE_FUNCTION
#else
    #define PROFILE_REGION(name) auto ANONYMOUS_VAR(name)              \
        = kc::core::Profiler::get()->createRegionTimer("Tag: " + std::string{name}); 

    #define PROFILE_FUNCTION() auto ANONYMOUS_VAR(__PRETTY_FUNCTION__) \
        = kc::core::Profiler::get()->createRegionTimer("Function: " + std::string{__PRETTY_FUNCTION__});
#endif
// clang-format on