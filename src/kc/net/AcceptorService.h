#pragma once

#include <vector>

#include "Session.hpp"
#include "kc/async/Acceptor.h"
#include "kc/async/Context.h"
#include "kc/model/MessageDeserializer.h"
#include "kc/service/Service.h"

namespace kc::net {

class AcceptorService : public service::Service {
    using Sessions = std::vector<Session>;

public:
    explicit AcceptorService(const std::string& name, async::Acceptor* acceptor,
        model::MessageDeserializer* deserializer)
        : m_acceptor(acceptor)
        , m_deserializer(deserializer) {
    }

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

protected:
    Sessions m_sessions;

private:
    async::Acceptor* m_acceptor;
    model::MessageDeserializer* deserializer;
};

}