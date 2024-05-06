#pragma once

#include <concepts>
#include <utility>

#include "allocators/Allocator.hpp"
#include "allocators/Mallocator.hpp"

namespace kc::mem {

template <typename T> class OwningPtr {
    template <typename C> friend class OwningPtr;

public:
    explicit OwningPtr() : m_allocator(nullptr), m_buffer(nullptr) {}

    ~OwningPtr() noexcept { clear(); }

    void clear() {
        if (m_buffer) {
            m_buffer->~T();
            m_allocator->deallocate(m_buffer, 1);
            m_buffer = nullptr;
        }
    }

    OwningPtr& operator=(OwningPtr&& oth) {
        if (m_buffer) m_allocator->deallocate(m_buffer, 1);

        m_buffer    = std::exchange(oth.m_buffer, nullptr);
        m_allocator = std::exchange(oth.m_allocator, nullptr);

        return *this;
    }

    template <typename F>
    requires std::derived_from<F, T> OwningPtr(OwningPtr<F>&& oth) {
        m_buffer    = std::exchange(oth.m_buffer, nullptr);
        m_allocator = std::exchange(oth.m_allocator, nullptr);
    }

    OwningPtr(OwningPtr&& oth) {
        m_buffer    = std::exchange(oth.m_buffer, nullptr);
        m_allocator = std::exchange(oth.m_allocator, nullptr);
    }

    OwningPtr(const OwningPtr&)            = delete;
    OwningPtr& operator=(const OwningPtr&) = delete;

    T& operator*() { return *m_buffer; }
    const T& operator*() const { return *m_buffer; }

    T* operator->() { return m_buffer; }
    const T* operator->() const { return m_buffer; }

    T* get() { return m_buffer; }
    const T* get() const { return m_buffer; }

    Allocator* getAllocator() { return m_allocator; }

    template <typename C, typename... Args>
    requires std::constructible_from<C, Args...>
    friend OwningPtr<C> makeOwningPtr(Allocator* allocator, Args&&... args);

    template <typename C, typename... Args>
    requires std::constructible_from<C, Args...>
    friend OwningPtr<C> makeOwningPtr(Args&&... args);

private:
    template <typename... Args>
    requires std::constructible_from<T, Args...>
    explicit OwningPtr(Allocator* allocator, Args&&... args) :
        m_allocator(allocator) {
        m_buffer = static_cast<T*>(m_allocator->allocate(1));
        new (m_buffer) T(std::forward<Args>(args)...);
    }

    inline static Mallocator<T> s_defaultAllocator;

    Allocator* m_allocator;
    T* m_buffer;
};

template <typename C, typename... Args>
requires std::constructible_from<C, Args...> OwningPtr<C> makeOwningPtr(
  Allocator* allocator, Args&&... args
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
