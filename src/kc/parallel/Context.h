#pragma once

#include <algorithm>
#include <ranges>
#include <vector>

#include "Core.h"
#include "kc/core/Log.h"

namespace kc::parallel {

class Context {
   public:
    explicit Context() {}

    void init() {
        getPlatforms();
        getDevices();

        for (const auto& device : m_devices) showDeviceInfo(device);

        new (&m_context) cl::Context(m_devices);
    }

    cl::Context& get() { return m_context; }

   private:
    void getPlatforms() {
        cl::Platform::get(&m_platforms);

        const auto platforms = m_platforms.size();
        if (platforms <= 0) throw ParallelError{"Could not find any platform supporting OpenCL"};

        LOG_INFO("Successfully loaded {} platforms", platforms);
    }

    void getDevices() {
        std::vector<cl::Device> devices;

        for (auto& platform : m_platforms) {
            platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);
            std::ranges::move(devices, std::back_inserter(m_devices));

            devices.clear();
        }

        if (m_devices.size() <= 0)
            throw ParallelError{"Could not find any device supporting OpenCL"};
    }

    void showDeviceInfo(const cl::Device& device) {
        LOG_INFO("Vendor: {}, Vendor ID: {}, Version: {}", device.getInfo<CL_DEVICE_VENDOR>(),
                 device.getInfo<CL_DEVICE_VENDOR_ID>(), device.getInfo<CL_DEVICE_VERSION>());
    }

    std::vector<cl::Platform> m_platforms;
    std::vector<cl::Device> m_devices;

    cl::Context m_context;
};

}  // namespace kc::parallel