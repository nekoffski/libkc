#pragma once

#include <vector>

#include "Context.h"
#include "Core.h"
#include "kc/core/Log.h"

namespace kc::parallel {

enum class BufferType : unsigned char { readOnly, writeOnly, readWrite };

template <typename T>
class Buffer {
   public:
    explicit Buffer(const int size, BufferType type, Context& context)
        : m_size(size), m_type(type), m_context(context) {
        m_buffer.resize(size, 0);

        auto& clContext = context.get();
    }

    cl::Buffer& get() { return m_clBuffer; }

    void readValuesFromGpu(cl::CommandQueue& queue) {
        ASSERT(m_type != BufferType::readOnly, "Cannot copy values of read only buffer");
        cl::copy(queue, m_clBuffer, m_buffer.begin(), m_buffer.end());
    }

    Buffer& fill(const T value) {
        ASSERT(m_type != BufferType::writeOnly, "Cannot fill write only buffer");
        std::ranges::fill(m_buffer, value);

        return *this;
    }

    void bind() {
        if (m_type == BufferType::writeOnly) {
            new (&m_clBuffer) cl::Buffer(m_context.get(), CL_MEM_WRITE_ONLY, sizeof(T) * m_size);
        } else {
            const bool isReadOnly = (m_type == BufferType::readOnly);
            new (&m_clBuffer)
                cl::Buffer(m_context.get(), m_buffer.begin(), m_buffer.end(), isReadOnly);
        }
    }

    auto begin() { return m_buffer.begin(); }
    auto end() { return m_buffer.end(); }

    const auto cbegin() const { return m_buffer.cbegin(); }
    const auto cend() const { return m_buffer.cend(); }

    const int getSize() const { return m_size; }
    const BufferType getType() const { return m_type; }

   private:
    BufferType m_type;
    const int m_size;

    Context& m_context;

    std::vector<T> m_buffer;
    cl::Buffer m_clBuffer;
};

}  // namespace kc::parallel
