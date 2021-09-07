#pragma once

#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace kc::core {

class FileSystem {
public:
    using Path = std::string;

    virtual bool isFile(const Path& path) const;
    virtual bool isDirectory(const Path& path) const;
    virtual std::vector<Path> listDirectory(const Path& path) const;

    virtual void writeFile(const Path& path, const std::string& buffer, bool override = false) const;
    virtual std::string readFile(const Path& path) const;

    virtual std::filesystem::file_time_type getLastFileModificationTime(const Path& path) const;
};

using FS = FileSystem;
}
