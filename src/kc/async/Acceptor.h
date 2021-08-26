#pragma once

#include <functional>
#include <memory>

#include <boost/asio.hpp>

#include "Context.h"
#include "Error.hpp"
#include "Socket.h"

#include "kc/core/Log.h"

namespace kc::async {

struct Acceptor {
    struct Factory {
        virtual std::unique_ptr<Acceptor> create(std::shared_ptr<Context> context, unsigned short port) = 0;
    };

    using AcceptCallback = std::function<void(async::Error, std::unique_ptr<Socket>)>;

    virtual ~Acceptor() = default;
    virtual void asyncAccept(AcceptCallback) = 0;

    virtual unsigned int getPort() const = 0;

    virtual std::shared_ptr<Context> getContext() = 0;
};

class AsioAcceptor : public Acceptor {
public:
    struct Factory : Acceptor::Factory {
        std::unique_ptr<Acceptor> create(std::shared_ptr<Context> context, unsigned short port) override {
            return std::make_unique<AsioAcceptor>(context, port);
        }
    };

    ~AsioAcceptor() override {
        m_acceptor->close();
    }

    explicit AsioAcceptor(std::shared_ptr<Context> context, unsigned short port)
        : m_port(port) {
        m_asioContext = std::dynamic_pointer_cast<AsioContext>(context);
        ASSERT(m_asioContext != nullptr, "Invalid async context");

        using namespace boost::asio::ip;

        m_acceptor = std::make_unique<tcp::acceptor>(
            m_asioContext->context, tcp::endpoint(tcp::v4(), port));
    }

    void asyncAccept(Acceptor::AcceptCallback acceptCallback) override {
        m_acceptor->async_accept([acceptCallback, this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket) {
            acceptCallback(Error { std::move(ec) }, std::make_unique<AsioSocket>(std::move(socket)));
        });
    }

    unsigned int getPort() const override {
        return m_port;
    }

    std::shared_ptr<Context> getContext() override {
        return m_asioContext;
    }

private:
    unsigned int m_port;

    std::shared_ptr<AsioContext> m_asioContext;
    std::unique_ptr<boost::asio::ip::tcp::acceptor> m_acceptor;
};

}