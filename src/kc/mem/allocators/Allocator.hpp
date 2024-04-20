#pragma once

#include <cstdint>

namespace kc::mem {

template <typename T> struct Allocator {
    [[nodiscard]] virtual T* allocate(std::size_t n)        = 0;
    virtual void deallocate(T* ptr, std::size_t n) noexcept = 0;
};

}  // namespace kc::mem
