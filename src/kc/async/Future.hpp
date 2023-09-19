#pragma once

#include <optional>
#include <memory>
#include <functional>
#include <mutex>
#include <thread>
#include <chrono>

namespace kc::async {

class ContextBase {
   public:
    explicit ContextBase() : m_valueSet(false) {}

    bool hasValue() const { return m_valueSet; }

   protected:
    std::atomic_bool m_valueSet;
    std::mutex m_mutex;
};

template <typename T> class Context : public ContextBase {
    using Callback = std::function<void(T)>;

   public:
    void setValue(T&& t) {
        std::unique_lock lock(m_mutex);
        m_valueSet = true;
        m_value    = std::forward<T>(t);
        if (this->m_callback) std::invoke(this->m_callback.value(), std::move(m_value.value()));
    }

    void setCallback(Callback&& callback) {
        std::unique_lock lock(m_mutex);
        m_callback = std::forward<Callback>(callback);
        if (m_valueSet) std::invoke(m_callback.value(), std::move(m_value.value()));
    }

    std::optional<T> m_value;
    std::optional<Callback> m_callback;
};

template <> class Context<void> : public ContextBase {
    using Callback = std::function<void()>;

   public:
    void setValue() {
        std::unique_lock lock(m_mutex);
        m_valueSet = true;
        if (this->m_callback) std::invoke(this->m_callback.value());
    }

    void setCallback(Callback&& callback) {
        std::unique_lock lock(m_mutex);
        m_callback = std::forward<Callback>(callback);
        if (m_valueSet) std::invoke(m_callback.value());
    }

    std::optional<Callback> m_callback;
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
        m_context->setCallback(std::forward<F>(f));
        return *this;
    }

   private:
    std::shared_ptr<Context<T>> m_context;
};

template <typename T> class PromiseBase {
   public:
    explicit PromiseBase(std::shared_ptr<Context<T>> context) : m_context(std::move(context)) {}

    template <typename F> void setCallback(F&& f) { m_context->setCallback(std::forward<F>(f)); }

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

}  // namespace kc::async
