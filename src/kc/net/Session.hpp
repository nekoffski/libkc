#pragma once

#include <chrono>
#include <memory>
#include <thread>

#include "kc/async/Socket.h"
#include "kc/core/Enumerate.hpp"
#include "kc/core/Log.h"
#include "kc/core/Scope.hpp"
#include "kc/core/Uuid.h"
#include "kc/json/Json.h"
#include "kc/model/Deserializable.h"
#include "kc/model/MessageDeserializer.h"
#include "kc/model/Serializable.h"

DEFINE_ERROR(SessionError);
DEFINE_SUB_ERROR(ResponseTimeout, SessionError);

namespace kc::net {

class Session {
    constexpr unsigned static int messageLengthBytes = 4u;

   public:
    void stop() {
        m_isConnected = false;
        m_socket->close();
    }

    explicit Session(
        std::unique_ptr<async::Socket> socket, model::MessageDeserializer* deserializer
    )
        : m_isConnected(true), m_socket(std::move(socket)), m_deserializer(deserializer) {
        readMessageLength();
    }

    bool isConnected() const { return m_isConnected; }

    template <typename T>
    requires std::derived_from<T, model::Serializable> std::string send(const T& message) {
        LOG_INFO("Starting sending message: {}", message.getName());

        auto jsonMessage = message.toMessageJson();

        if (not jsonMessage.isMember("conversation-id")) {
            LOG_INFO("Message is not part of any conversation, creating UUID for new conversation");
            jsonMessage["conversation-id"] = core::generateUuid();
        }

        sendMessageLength(json::toString(jsonMessage));

        return jsonMessage["conversation-id"].asString();
    }

    std::unique_ptr<model::Deserializable> waitForResponse(const std::string& conversationId) {
        using clock = std::chrono::steady_clock;
        using namespace std::chrono_literals;

        auto timeout = 5s;
        auto start   = clock::now();

        // TODO: protect with mutex
        while (start + timeout >= clock::now()) {
            if (not m_isConnected) throw SessionError{"Connection dropped"};

            if (m_conversations.contains(conversationId)) {
                ON_SCOPE_EXIT { m_conversations.erase(conversationId); };

                return std::move(m_conversations[conversationId]);
            }

            std::this_thread::yield();
            std::this_thread::sleep_for(5ms);
        }

        throw ResponseTimeout{"Response timed out"};
    }

    bool isEmpty() const { return m_receivedMessages.empty(); }

    std::unique_ptr<model::Deserializable> getMessage() {
        ON_SCOPE_EXIT { m_receivedMessages.pop_back(); };

        return std::move(m_receivedMessages.back());
    }

   private:
    void sendMessageLength(const std::string& message) {
        LOG_INFO("Trying to send message length");

        auto messageLength = message.size();

        auto onWrite = [&, message](async::Error error, [[maybe_unused]] unsigned int bytesSent) {
            if (error) {
                m_isConnected = false;
                return;
            }
            sendMessage(message);
        };

        auto messageLengthAsBytes =
            std::string{reinterpret_cast<char*>(&messageLength), messageLengthBytes};
        m_socket->asyncWrite(messageLengthAsBytes, onWrite);
    }

    void readMessageLength() {
        auto onRead = [&](async::Error error, std::string message, unsigned int bytesReceived) {
            LOG_INFO("Reading message length");

            if (bytesReceived != messageLengthBytes) {
                LOG_WARN("Received invalid number of bytes");
                return;
            }

            if (error) {
                LOG_WARN("Could not read message length due to: {}", error.asString());
                m_isConnected = false;
                return;
            }

            unsigned int messageLength = *reinterpret_cast<unsigned int*>(message.data());
            readMessage(messageLength);
        };

        m_socket->asyncRead(onRead, messageLengthBytes);
    }

    void sendMessage(const std::string& message) {
        if (not m_isConnected) throw SessionError{"Connection dropped"};

        LOG_INFO("Sending message: {}", message);
        m_socket->asyncWrite(message, [&](async::Error error, unsigned int bytesSent) {
            if (error) {
                LOG_WARN("Could not send message due to: {}", error.asString());
                m_isConnected = false;
                return;
            }
        });
    }

    void readMessage(unsigned int messageLength) {
        auto onMessage = [&](async::Error error, std::string message, unsigned int bytesRead) {
            ON_SCOPE_EXIT { readMessageLength(); };

            if (error) {
                LOG_WARN("Could not read message due to: {}", error.asString());
                m_isConnected = false;
                return;
            }

            LOG_INFO("Received message: {}", message);
            processMessage(std::move(message));
            LOG_INFO("Message processed");
        };

        // TODO: read message size first
        m_socket->asyncRead(onMessage, messageLength);
    }

    void processMessage(std::string message) {
        try {
            const auto json        = json::loadJson(message);
            const auto messageName = json["name"].asString();

            LOG_INFO("Received message with header: {}", messageName);

            auto message = m_deserializer->deserializeMessage(messageName, json["body"]);

            if (not message) {
                LOG_WARN("Received unknown message: {}", messageName);
                return;
            }

            LOG_INFO("Message deserialized successfully");

            if (json.isMember("conversation-id")) {
                auto conversationId = json["conversation-id"].asString();
                m_conversations.insert({conversationId, std::move(message)});
            } else {
                m_receivedMessages.emplace_back(std::move(message));
            }
        } catch (core::ErrorBase& error) {
            LOG_ERROR("Could not process message: {} due to: {}", message, error.asString());
        }
    }

    bool m_isConnected;

    std::unique_ptr<async::Socket> m_socket;
    model::MessageDeserializer* m_deserializer;

    std::vector<std::unique_ptr<model::Deserializable>> m_receivedMessages;
    std::unordered_map<std::string, std::unique_ptr<model::Deserializable>> m_conversations;
};

}  // namespace kc::net