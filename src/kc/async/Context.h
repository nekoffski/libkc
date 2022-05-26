#pragma once

#include <kc/core/Log.h>

#include <boost/asio.hpp>
#include <thread>

namespace kc::async {

struct Context {
    virtual void runThreaded() = 0;
    virtual void stop()        = 0;

    virtual ~Context() = default;
};

class AsioContext : public Context {
   public:
    void runThreaded() override {
        LOG_INFO("Creating thread for Asio Context");
        m_contextThread = std::make_unique<std::jthread>([&]() {
            LOG_INFO("Running Asio Context from a separated thread");
            context.run();
        });
    }

    void stop() override {
        LOG_INFO("Stopping Asio Context");
        context.stop();
    }

    boost::asio::io_context context;

   private:
    std::unique_ptr<std::jthread> m_contextThread;
};

}  // namespace kc::async