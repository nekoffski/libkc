#pragma once

#include <boost/asio.hpp>
#include <functional>
#include <memory>

#include "Context.h"
#include "Error.hpp"
#include "Socket.h"
#include "kc/core/Log.h"

namespace kc::async {

class Acceptor {
   public:
    explicit Acceptor(unsigned short port) : m_port(port) {}

    using AcceptCallback = std::function<void(async::Error, std::unique_ptr<Socket>)>;

    virtual ~Acceptor() = default;
    virtual void asyncAccept(AcceptCallback) = 0;

    unsigned int getPort() const { return m_port; }

    virtual Context* getContext() = 0;

   protected:
    // Context* m_context;
    unsigned int m_port;
};

using namespace boost::asio::ip;

class AsioAcceptor : public Acceptor {
   public:
    using Acceptor::Acceptor;

    ~AsioAcceptor() override {
        m_acceptor.close();
        m_asioContext.stop();
    }

    explicit AsioAcceptor(unsigned short port)
        : Acceptor(port), m_acceptor(m_asioContext.context, tcp::endpoint(tcp::v4(), port)) {}

    void asyncAccept(Acceptor::AcceptCallback acceptCallback) override {
        m_acceptor.async_accept([acceptCallback, this](boost::system::error_code ec,
                                                       boost::asio::ip::tcp::socket socket) {
            acceptCallback(Error{std::move(ec)}, std::make_unique<AsioSocket>(std::move(socket)));
        });
    }

    Context* getContext() override { return &m_asioContext; }

   private:
    AsioContext m_asioContext;
    boost::asio::ip::tcp::acceptor m_acceptor;
};

}  // namespace kc::async