#pragma once

#include <memory>

#include "kc/async/Socket.h"
#include "kc/core/Log.h"
#include "kc/core/Scope.hpp"
#include "kc/core/Uuid.h"
#include "kc/json/Json.h"
#include "kc/model/Deserializable.h"
#include "kc/model/MessageDeserializer.h"
#include "kc/model/Serializable.h"

namespace kc::net {

class Session {

public:
    explicit Session(std::unique_ptr<async::Socket> socket, model::MessageDeserializer* deserializer)
        : m_socket(std::move(socket))
        , m_deserializer(deserializer) {
        waitForMessage();
    }

    template <typename T>
    requires std::derived_from<T, model::Serializable> void send(T&& message) {
        auto jsonMessage = message.toJson();

        if (not jsonMessage.isMember("conversation-id"))
            jsonMessage["conversation-id"] = core::generateUuid();

        sendMessage(json::toString(jsonMessage))
    }

    std::unique_ptr<model::Deserializable> waitForResponse(const std::string& conversationId) {
        LOOP {
            if (m_conversations.contains(conversationId)) {
                ON_SCOPE_EXIT { m_conversations.erase(conversationId); };
                return std::move(m_conversations[conversationId]);
            }

            // async_sleep
        }
        // handle timeout!
    }

    bool isEmpty() const {
        return m_receivedMessages.empty();
    }

    std::unique_ptr<model::Deserializable> getMessage() {
        ON_SCOPE_EXIT { m_receivedMessages.pop_back(); };

        return std::move(m_receivedMessages.back());
    }

private:
    void sendMessage(const std::string& message) {
    }

    void waitForMessage() {
        auto onMessage = [&](async::Error error, std::string message, unsigned int bytesRead) {
            ON_SCOPE_EXIT { waitForMessage(); };

            if (error) {
                LOG_WARN("Could not read message due to: {}", error.asString());
                return;
            }

            processMessage(std::move(message));
        };

        // TODO: read message size first
        m_socket->asyncRead(onMessage, 1024);
    }

    void processMessage(std::string message) {
        const auto json = json::loadJson(message);
        const auto messageName = json["name"].asString();

        auto message = m_deserializer->deserializeMessage(messageName, json["body"]);

        if (json.isMember("conversation-id")) {
            auto conversationId = json["conversation-id"].asString();
            m_conversations.insert(conversationId, std::move(message));
        } else {
            m_receivedMessages.emplace_back(std::move(message));
        }
    }

    std::unique_ptr<async::Socket> m_socket;
    model::MessageDeserializer* m_deserializer;

    std::vector<std::unique_ptr<model::Deserializable>> m_receivedMessages;
    std::unordered_map<std::string, std::unique_ptr<model::Deserializable>> m_conversations;
};

}