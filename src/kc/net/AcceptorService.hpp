#pragma once

#include <vector>

#include "Session.hpp"
#include "kc/async/Acceptor.h"
#include "kc/async/Context.h"
#include "kc/model/MessageDeserializer.h"
#include "kc/service/Service.h"

namespace kc::net {

template <typename Acceptor>
requires std::derived_from<Acceptor, async::Acceptor> class AcceptorService : public service::Service {
    using Sessions = std::vector<Session>;

public:
    explicit AcceptorService(const std::string& name, model::MessageDeserializer* deserializer)
        : Service(name)
        , m_deserializer(deserializer) {
    }

    void startAccepting(async::Context* context, unsigned int port) {
        m_acceptor = std::make_unique<Acceptor>(context, port);
        waitForConnection();
    }

protected:
    Sessions m_sessions;

private:
    void waitForConnection() {
        m_acceptor->asyncAccept([&](async::Error error, std::unique_ptr<async::Socket> socket) {
            ON_SCOPE_EXIT { waitForConnection(); };

            if (error) {
                LOG_WARN("Could not accept connection due to: {}", error.asString());
                return;
            }

            LOG_INFO("Received connection, creating session");
            m_sessions.emplace_back(std::move(socket), m_deserializer);
        });
    }

    std::unique_ptr<Acceptor> m_acceptor;
    model::MessageDeserializer* m_deserializer;
};

}