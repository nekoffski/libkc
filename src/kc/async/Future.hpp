#pragma once

#include <memory>

#include <memory>
#include <functional>
#include <mutex>
#include <future>

namespace kc::async {

template <typename T> class ContextBase {
   public:
    std::future<T> getFuture() { return m_promise.get_future(); }

   protected:
    std::promise<T> m_promise;
    std::mutex m_mutex;
};

template <typename T> class Context : public ContextBase<T> {
   public:
    void setValue(const T& value) { this->m_promise.set_value(value); }
};

template <> class Context<void> : public ContextBase<void> {
   public:
    void setValue() { this->m_promise.set_value(); }
};

template <typename T> class Future {
   public:
    explicit Future(std::shared_ptr<Context<T>> context)
        : m_context(std::move(context)), m_future(m_context->getFuture()) {}

    void wait() { m_future.wait(); }

   private:
    std::shared_ptr<Context<T>> m_context;
    std::future<T> m_future;
};

template <typename T> class PromiseBase {
   public:
    explicit PromiseBase(std::shared_ptr<Context<T>> context) : m_context(std::move(context)) {}

    // template <typename F> void setCallback(F&& f) { m_callback = std::forward<F>(f); }

   protected:
    std::shared_ptr<Context<T>> m_context;
};

template <typename T> class Promise : public PromiseBase<T> {
   public:
    using PromiseBase<T>::PromiseBase;

    void setValue(const T& value) { this->m_context->setValue(value); }
};

template <> class Promise<void> : public PromiseBase<void> {
    using PromiseBase<void>::PromiseBase;

   public:
    void setValue() { this->m_context->setValue(); }
};

template <typename... F> void wait(Future<F>&... futures) { (futures.wait(), ...); }

}  // namespace kc::async
