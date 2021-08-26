#pragma once

#include <thread>

#include <boost/asio.hpp>

namespace kc::async {

struct Context {
    struct Factory {
        virtual std::shared_ptr<Context> create() = 0;
    };

    virtual void runThreaded() = 0;
    virtual void stop() = 0;
};

class AsioContext : public Context {
public:
    struct Factory : public Context::Factory {
        std::shared_ptr<Context> create() override {
            return std::make_shared<AsioContext>();
        }
    };

    void runThreaded() override {
        m_contextThread = std::make_unique<std::jthread>([&]() {
            context.run();
        });
    }

    void stop() override {
        context.stop();
    }

    boost::asio::io_context context;

private:
    std::unique_ptr<std::jthread> m_contextThread;
};

}