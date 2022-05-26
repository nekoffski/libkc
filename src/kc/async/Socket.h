#pragma once

#include <boost/asio.hpp>
#include <functional>

#include "Error.hpp"

namespace kc::async {

struct Socket {
    Socket() = default;

    Socket(const Socket&)            = delete;
    Socket& operator=(const Socket&) = delete;

    using ReadCallback  = std::function<void(Error, std::string message, unsigned int)>;
    using WriteCallback = std::function<void(Error, unsigned int)>;

    virtual void asyncRead(ReadCallback&&, unsigned int bytesToRead) = 0;
    virtual void asyncWrite(const std::string&, WriteCallback&&)     = 0;

    virtual void close() = 0;
};

class AsioSocket : public Socket {
   public:
    explicit AsioSocket(boost::asio::ip::tcp::socket socket) : m_socket(std::move(socket)) {}

    AsioSocket(AsioSocket&&)            = default;
    AsioSocket& operator=(AsioSocket&&) = default;

    void asyncRead(Socket::ReadCallback&& readCallback, unsigned int bytesToRead) override {
        m_buffer.clear();
        m_buffer.resize(1024, '\0');

        auto onRead = [readCallback, this](boost::system::error_code ec, std::size_t length) {
            m_buffer.resize(length);
            readCallback(Error{std::move(ec)}, m_buffer, length);
        };

        m_socket.async_receive(boost::asio::buffer(m_buffer, bytesToRead), onRead);
    }

    void asyncWrite(const std::string& message, WriteCallback&& writeCallback) {
        auto onWrite = [writeCallback, this](boost::system::error_code ec, std::size_t length) {
            writeCallback(Error{std::move(ec)}, length);
        };

        auto messageSize = message.size();
        m_socket.async_write_some(boost::asio::buffer(message, messageSize), onWrite);
    }

    void close() override { m_socket.close(); }

   private:
    std::string m_buffer;
    boost::asio::ip::tcp::socket m_socket;
};

}  // namespace kc::async