#pragma once

#include <concepts>
#include <utility>

#include "allocators/Allocator.hpp"
#include "allocators/Mallocator.hpp"

namespace kc::mem {

template <typename T> class OwningPtr {
public:
    explicit OwningPtr() : m_allocator(nullptr), m_buffer(nullptr) {}

    ~OwningPtr() noexcept {
        if (m_buffer) {
            m_buffer->~T();
            m_allocator->deallocate(m_buffer, 1);
        }
    }

    OwningPtr& operator=(OwningPtr&& oth) {
        if (m_buffer) m_allocator->deallocate(m_buffer, 1);

        m_buffer    = std::exchange(oth.m_buffer, nullptr);
        m_allocator = std::exchange(oth.m_allocator, nullptr);

        return *this;
    }

    OwningPtr(OwningPtr&& oth) { *this = std::move(oth); }

    OwningPtr(const OwningPtr&)            = delete;
    OwningPtr& operator=(const OwningPtr&) = delete;

    T& operator*() { return *m_buffer; }
    const T& operator*() const { return *m_buffer; }

    T* operator->() { return m_buffer; }
    const T* operator->() const { return m_buffer; }

    T* get() { return m_buffer; }
    const T* get() const { return m_buffer; }

    Allocator<T>* getAllocator() { return m_allocator; }

    template <typename C, typename... Args>
    requires std::constructible_from<C, Args...>
    friend OwningPtr<C> makeOwningPtr(Allocator<C>* allocator, Args&&... args);

    template <typename C, typename... Args>
    requires std::constructible_from<C, Args...>
    friend OwningPtr<C> makeOwningPtr(Args&&... args);

private:
    template <typename... Args>
    requires std::constructible_from<T, Args...>
    explicit OwningPtr(Allocator<T>* allocator, Args&&... args) :
        m_allocator(allocator) {
        m_buffer = m_allocator->allocate(1);
        new (m_buffer) T(std::forward<Args>(args)...);
    }

    inline static Mallocator<T> s_defaultAllocator;

    Allocator<T>* m_allocator;
    T* m_buffer;
};

template <typename C, typename... Args>
requires std::constructible_from<C, Args...> OwningPtr<C> makeOwningPtr(
  Allocator<C>* allocator, Args&&... args
) {
    return OwningPtr<C>{ allocator, std::forward<Args>(args)... };
}

template <typename C, typename... Args>
requires std::constructible_from<C, Args...> OwningPtr<C> makeOwningPtr(
  Args&&... args
) {
    return OwningPtr<C>{
        &OwningPtr<C>::s_defaultAllocator, std::forward<Args>(args)...
    };
}

}  // namespace kc::mem
