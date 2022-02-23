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
    explicit Acceptor(Context* context, unsigned short port) : m_context(context), m_port(port) {}

    using AcceptCallback = std::function<void(async::Error, std::unique_ptr<Socket>)>;

    virtual ~Acceptor() = default;
    virtual void asyncAccept(AcceptCallback) = 0;

    unsigned int getPort() const { return m_port; }

    Context* getContext() const { return m_context; }

   protected:
    Context* m_context;
    unsigned int m_port;
};

class AsioAcceptor : public Acceptor {
   public:
    using Acceptor::Acceptor;

    ~AsioAcceptor() override { m_acceptor->close(); }

    explicit AsioAcceptor(Context* context, unsigned short port) : Acceptor(context, port) {
        m_asioContext = dynamic_cast<AsioContext*>(m_context);

        ASSERT(m_asioContext != nullptr, "Invalid async context");

        using namespace boost::asio::ip;

        m_acceptor =
            std::make_unique<tcp::acceptor>(m_asioContext->context, tcp::endpoint(tcp::v4(), port));
    }

    void asyncAccept(Acceptor::AcceptCallback acceptCallback) override {
        m_acceptor->async_accept([acceptCallback, this](boost::system::error_code ec,
                                                        boost::asio::ip::tcp::socket socket) {
            acceptCallback(Error{std::move(ec)}, std::make_unique<AsioSocket>(std::move(socket)));
        });
    }

   private:
    AsioContext* m_asioContext;
    std::unique_ptr<boost::asio::ip::tcp::acceptor> m_acceptor;
};

}  // namespace kc::async