#pragma once

#include <chrono>
#include <concepts>
#include <functional>
#include <future>
#include <memory>
#include <string>
#include <typeindex>

#include "Category.h"
#include "Error.h"

using namespace std::literals::chrono_literals;

namespace kc::event {

class EventEmitter;

struct Event : std::enable_shared_from_this<Event> {
public:
    friend class EventEmitter;

    virtual std::type_index getEventTypeIndex() const = 0;
    virtual std::type_index getCategoryTypeIndex() const = 0;

    template <typename T>
    void on(std::function<void(std::shared_ptr<T>)> callback) {
        if (typeid(T) == getEventTypeIndex())
            callback(std::dynamic_pointer_cast<T>(shared_from_this()));
    }

    virtual std::string asString() const {
        return "Name not specified";
    }

    template <typename T>
    bool is() {
        return typeid(T) == getEventTypeIndex();
    }

    template <typename T>
    std::shared_ptr<T> as() {
        return std::dynamic_pointer_cast<T>(shared_from_this());
    }

    template <typename T>
    T* asView() {
        return static_cast<T*>(this);
    }

    template <typename T, typename... Args>
    void setResult(Args&&... args) {
        m_result.set_value(std::make_shared<T>(std::forward<Args>(args)...));
    }

private:
    std::promise<std::shared_ptr<Event>> m_result;
};

class Result {
public:
    explicit Result(std::future<std::shared_ptr<Event>> resultFuture)
        : m_resultFuture(std::move(resultFuture)) {
    }

    std::shared_ptr<Event> getValue() {
        return m_resultFuture.get();
    }

    bool isReady() {
        return m_resultFuture.wait_for(0s) == std::future_status::ready;
    }

    std::shared_ptr<Event> wait(const std::chrono::milliseconds timeout = 2s) {
        if (m_resultFuture.wait_for(timeout) == std::future_status::ready)
            return m_resultFuture.get();
        throw ResultTimeout {};
    }

private:
    std::future<std::shared_ptr<Event>> m_resultFuture;
};

template <typename T>
concept EventType = std::derived_from<T, Event>;

// clang-format off
template <typename Ev, CategoryType Category = DefaultCategory>
struct EventBase : public Event {
    std::type_index getEventTypeIndex() const final override {
        return typeid(Ev);
    }

    std::type_index getCategoryTypeIndex() const final override {
        return typeid(Category);
    }
};
// clang-format on
}
