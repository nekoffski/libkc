#pragma once

#include <gmock/gmock.h>

#include "kc/core/FileSystem.h"

namespace kc::core {

struct FileSystemMock : public FileSystem {
    MOCK_METHOD(bool, isFile, (const Path& path), (const, override));
    MOCK_METHOD(bool, isDirectory, (const Path& path), (const, override));
    MOCK_METHOD(std::vector<Path>, listDirectory, (const Path& path), (const, override));
    MOCK_METHOD(void, writeFile,
                (const Path& path, const std::string& buffer, FileSystem::WritePolicy),
                (const, override));
    MOCK_METHOD(std::string, readFile, (const Path& path), (const, override));
    MOCK_METHOD(std::filesystem::file_time_type, getLastFileModificationTime, (const Path& path),
                (const, override));
};

}  // namespace kc::core