#pragma once

#include "Context.h"
#include "Core.h"
#include "kc/core/FileSystem.h"

namespace kc::parallel {

template <typename... Args> class Kernel {
   public:
    explicit Kernel(Context& context, const std::string& source, const std::string& name)
        : m_program(context.get(), source, true), m_kernel(m_program, name) {}

    static Kernel fromFile(
        Context& context, const std::string& path, const std::string& name,
        const core::FileSystem& fileSystem = core::FileSystem{}
    ) {
        if (not fileSystem.isFile(path)) throw ParallelError{"Could not find kernel source file"};

        return Kernel{context, fileSystem.readFile(path), name};
    }

    cl::KernelFunctor<Args...>& get() { return m_kernel; }

   private:
    cl::Program m_program;
    cl::KernelFunctor<Args...> m_kernel;
};

}  // namespace kc::parallel