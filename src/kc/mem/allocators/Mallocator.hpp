#pragma once

#include <limits>
#include <new>

#include "kc/core/Log.h"

#include "Allocator.hpp"

namespace kc::mem {

template <typename T> class Mallocator : public Allocator {
public:
    using ValueType = T;

    explicit Mallocator() = default;

    [[nodiscard]] void* allocate(std::size_t n) override {
        if (n > std::numeric_limits<std::size_t>::max() / sizeof(T))
            throw std::bad_alloc();

        if (auto p = static_cast<T*>(std::malloc(n * sizeof(T)))) {
            report(p, n);
            return p;
        }

        throw std::bad_alloc();
    }

    void deallocate(void* p, std::size_t n) noexcept override {
        report(p, n, false);
        std::free(static_cast<T*>(p));
    }

private:
    void report(void* p, std::size_t n, bool alloc = true) const {
        LOG_INFO(
          "{} '{}' bytes at '{}'",
          alloc ? "Allocating:" : "Deallocating:", sizeof(T) * n, p
        );
    }
};

}  // namespace kc::mem
