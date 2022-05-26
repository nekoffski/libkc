#pragma once

#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "kc/core/String.h"

namespace kc::core {

class FileSystem {
   public:
    enum class WritePolicy { override, noOverride };

    using Path = std::string;

    virtual bool isFile(const Path& path) const;
    virtual bool isDirectory(const Path& path) const;
    virtual std::vector<Path> listDirectory(const Path& path) const;

    virtual void writeFile(
        const Path& path, const std::string& buffer,
        WritePolicy writePolicy = WritePolicy::noOverride
    ) const;
    virtual std::string readFile(const Path& path) const;

    virtual std::vector<std::string> readLines(const Path& path) const;

    virtual std::filesystem::file_time_type getLastFileModificationTime(const Path& path) const;
};

using FS = FileSystem;
}  // namespace kc::core
