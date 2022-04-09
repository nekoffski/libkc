#pragma once

#include <vector>

#include "Session.hpp"
#include "kc/async/Acceptor.h"
#include "kc/async/Context.h"
#include "kc/model/MessageDeserializer.h"
#include "kc/service/PeriodicService.h"

namespace kc::net {

template <typename Acceptor>
requires std::derived_from<Acceptor, async::Acceptor>
class AcceptorService : public service::PeriodicService {
   public:
    explicit AcceptorService(const std::string& name, const unsigned int port)
        : PeriodicService(name), m_acceptor(port) {}

    void startAccepting() {
        m_acceptor.getContext()->runThreaded();
        waitForConnection();
    }

   private:
    void waitForConnection() {
        LOG_TRACE("Waiting for connection async");
        m_acceptor.asyncAccept([&](async::Error error, std::unique_ptr<async::Socket> socket) {
            ON_SCOPE_EXIT { waitForConnection(); };

            if (error) {
                LOG_WARN("Could not accept connection due to: {}", error.asString());
                return;
            }

            LOG_INFO("Received connection, creating session");
            onConnection(std::move(socket));
        });
    }

    virtual void onConnection(std::unique_ptr<async::Socket> socket) = 0;

    Acceptor m_acceptor;
};

}  // namespace kc::net