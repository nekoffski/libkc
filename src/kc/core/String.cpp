#include "String.h"

#include <iostream>

namespace kc::core {

void strip(std::string& str, const char characterToStrip) {
    auto n = str.size() - 1;

    while (n > 0 && str[n] == characterToStrip) n--;

    str.erase(n + 1);
}

std::vector<std::string> split(const std::string& str, const char delimiter) {
    std::vector<std::string> output;
    const auto n = str.size();

    if (n > 0) {
        int beginIndex = 0;

        for (int i = 0; i < n; ++i) {
            if (str[i] == delimiter) {
                if (int characters = i - beginIndex; characters > 0)
                    output.push_back(str.substr(beginIndex, characters));

                while (str[i + 1] == delimiter) ++i;

                beginIndex = i + 1;
            }
        }
        if (beginIndex != n) output.push_back(str.substr(beginIndex, n - beginIndex));
    }
    return output;
}

std::string extractNameFromPath(const std::string& path, ExtractingMode mode) {
    static constexpr char separator = '/';
    const auto& npos                = std::string::npos;

    const auto position = path.find_last_of(separator);
    const bool isPath   = position != npos;

    if (not isPath) return path;

    if (mode == ExtractingMode::withExtension) return path.substr(position + 1);

    static constexpr char dot = '.';

    auto beginOfExtension   = path.find_last_of(dot);
    const bool hasExtension = beginOfExtension != npos;
    const auto n            = path.size();

    return hasExtension ? path.substr(position + 1, beginOfExtension - position - 1)
                        : path.substr(position + 1);
}

}  // namespace kc::core