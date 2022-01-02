#pragma once

#include <chrono>
#include <memory>
#include <thread>

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
    constexpr unsigned static int messageLengthBytes = 4u;

public:
    explicit Session(std::unique_ptr<async::Socket> socket, model::MessageDeserializer* deserializer)
        : m_socket(std::move(socket))
        , m_deserializer(deserializer) {
        readMessageLength();
    }

    template <typename T>
    requires std::derived_from<T, model::Serializable> std::string send(T&& message) {
        LOG_INFO("Starting sending message: {}", message.getName());

        auto jsonMessage = message.toJson();

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
        auto start = clock::now();

        // TODO: protect with mutex
        while (start + timeout >= clock::now()) {
            if (m_conversations.contains(conversationId)) {
                ON_SCOPE_EXIT { m_conversations.erase(conversationId); };

                return std::move(m_conversations[conversationId]);
            }

            std::this_thread::yield();
            std::this_thread::sleep_for(5ms);
        }
    }

    bool isEmpty() const {
        return m_receivedMessages.empty();
    }

    std::unique_ptr<model::Deserializable> getMessage() {
        ON_SCOPE_EXIT { m_receivedMessages.pop_back(); };

        return std::move(m_receivedMessages.back());
    }

private:
    void sendMessageLength(const std::string& message) {
        LOG_INFO("Trying to send message length");

        auto messageLength = message.size();

        auto onWrite = [&, message](async::Error error, unsigned int bytesSent) {
            if (error) {
            }
            sendMessage(message);
        };

        auto messageLengthAsBytes =
            std::string { reinterpret_cast<char*>(&messageLength), messageLengthBytes };
        m_socket->asyncWrite(messageLengthAsBytes, onWrite);
    }

    void readMessageLength() {
        auto onRead = [&](async::Error error, std::string message, unsigned int bytesReceived) {
            LOG_INFO("Reading message length");

            if (bytesReceived != messageLengthBytes) {
            }

            if (error) {
            }

            if (bytesReceived == 0) {
                // return;
            }

            unsigned int messageLength = *reinterpret_cast<unsigned int*>(message.data());

            waitForMessage(messageLength);
        };

        m_socket->asyncRead(onRead, messageLengthBytes);
    }

    void sendMessage(const std::string& message) {
        LOG_INFO("Sending message: {}", message);
        m_socket->asyncWrite(message, [](async::Error error, unsigned int bytesSent) {
            if (error)
                LOG_WARN("Could not send message due to: {}", error.asString());
        });
    }

    void waitForMessage(unsigned int messageLength) {
        auto onMessage = [&](async::Error error, std::string message, unsigned int bytesRead) {
            ON_SCOPE_EXIT { readMessageLength(); };

            if (error) {
                LOG_WARN("Could not read message due to: {}", error.asString());
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
            const auto json = json::loadJson(message);
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
                m_conversations.insert({ conversationId, std::move(message) });
            } else {
                m_receivedMessages.emplace_back(std::move(message));
            }
        } catch (core::ErrorBase& error) {
            LOG_ERROR("Could not process message: {} due to: {}", message, error.asString());
        }
    }

    std::unique_ptr<async::Socket> m_socket;
    model::MessageDeserializer* m_deserializer;

    std::vector<std::unique_ptr<model::Deserializable>> m_receivedMessages;
    std::unordered_map<std::string, std::unique_ptr<model::Deserializable>> m_conversations;
};

}