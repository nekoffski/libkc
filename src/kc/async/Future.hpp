#pragma once

#include "kc/core/Log.h"

#include <vector>
#include <optional>
#include <memory>
#include <functional>
#include <mutex>
#include <thread>
#include <chrono>

namespace kc::async {

template <typename Callback> class ContextBase {
   public:
    explicit ContextBase() : m_valueSet(false) {}

    bool hasValue() const {
        std::unique_lock lock(m_mutex);
        return m_valueSet;
    }

    bool hasCallback() const {
        std::unique_lock lock(m_mutex);
        return m_callback.has_value();
    }

   protected:
    std::atomic_bool m_valueSet;

    mutable std::mutex m_mutex;

    std::optional<Callback> m_callback;
};

template <typename T> class Context : public ContextBase<std::function<void(T)>> {
    using Callback = std::function<void(T)>;

   public:
    void setValue(T&& t) {
        std::unique_lock lock(this->m_mutex);
        this->m_valueSet = true;
        m_value          = std::forward<T>(t);
        if (this->m_callback) executeCallback();
    }

    void setCallback(Callback&& callback) {
        std::unique_lock lock(this->m_mutex);
        this->m_callback = std::forward<Callback>(callback);
        if (this->m_valueSet) executeCallback();
    }

    T getValue() {
        std::unique_lock lock(this->m_mutex);

        ASSERT(not m_valueConsumed, "Value has been already consumed");
        m_valueConsumed = true;
        return std::move(m_value.value());
    }

    std::atomic_bool m_valueConsumed = false;
    std::optional<T> m_value;

   private:
    void executeCallback() {
        m_valueConsumed = true;
        std::invoke(this->m_callback.value(), std::move(m_value.value()));
    }
};

template <> class Context<void> : public ContextBase<std::function<void()>> {
    using Callback = std::function<void()>;

   public:
    void setValue() {
        std::unique_lock lock(this->m_mutex);
        this->m_valueSet = true;
        if (this->m_callback) std::invoke(this->m_callback.value());
    }

    void setCallback(Callback&& callback) {
        std::unique_lock lock(m_mutex);
        this->m_callback = std::forward<Callback>(callback);
        if (this->m_valueSet) std::invoke(this->m_callback.value());
    }
};

template <typename T> class Future {
   public:
    explicit Future(std::shared_ptr<Context<T>> context) : m_context(std::move(context)) {}

    void wait() {
        using namespace std::chrono_literals;

        // TODO: change to condition variable?
        while (not m_context->hasValue()) std::this_thread::sleep_for(10ms);
    }

    template <typename F> Future& then(F&& f) {
        ASSERT(not m_context->hasCallback(), "Callback already set, cannot set it twice");
        m_context->setCallback(std::forward<F>(f));
        return *this;
    }

    T get() {
        wait();
        return m_context->getValue();
    }

   private:
    std::shared_ptr<Context<T>> m_context;
};

template <typename T> class PromiseBase {
   public:
    explicit PromiseBase(std::shared_ptr<Context<T>> context) : m_context(std::move(context)) {}

   protected:
    std::shared_ptr<Context<T>> m_context;
};

template <typename T> class Promise : public PromiseBase<T> {
   public:
    using PromiseBase<T>::PromiseBase;

    void setValue(T&& value) { this->m_context->setValue(std::move(value)); }
};

template <> class Promise<void> : public PromiseBase<void> {
    using PromiseBase<void>::PromiseBase;

   public:
    void setValue() { this->m_context->setValue(); }
};

template <typename... F> void wait(Future<F>&... futures) { (futures.wait(), ...); }

template <typename T> void wait(std::vector<Future<T>>& futures) {
    for (auto& future : futures) future.wait();
}

template <typename T> class FutureCollection {
   public:
    explicit FutureCollection(std::vector<Future<T>>&& futures) : m_futures(std::move(futures)) {}

    void wait() { async::wait(m_futures); }

    FutureCollection(const FutureCollection& oth)            = delete;
    FutureCollection& operator=(const FutureCollection& oth) = delete;

   private:
    std::vector<Future<T>> m_futures;
};

}  // namespace kc::async
