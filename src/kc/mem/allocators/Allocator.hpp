#pragma once

#include <cstdint>

namespace kc::mem {

struct Allocator {
    [[nodiscard]] virtual void* allocate(std::size_t n)        = 0;
    virtual void deallocate(void* ptr, std::size_t n) noexcept = 0;
};

}  // namespace kc::mem
