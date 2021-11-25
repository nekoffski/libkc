#include "String.h"

namespace kc::core {

void strip(std::string& str, const char characterToStrip) {
    auto n = str.size() - 1;

    while (n > 0 && str[n] == characterToStrip)
        n--;

    str.erase(n + 1);
}

std::vector<std::string> split(const std::string& str, const char delimiter) {
    std::vector<std::string> output;
    const auto n = str.size();

    if (n > 0) {
        int beginIndex = 0;

        for (int i = 0; i < n; ++i) {
            if (str[i] == delimiter) {

                if (int end = i - beginIndex; beginIndex != end)
                    output.push_back(str.substr(beginIndex, end));

                beginIndex = i + 1;
            }
        }
        if (beginIndex != n)
            output.push_back(str.substr(beginIndex, n - beginIndex));
    }
    return output;
}

std::string extractNameFromPath(const std::string& path) {
    static constexpr char separator = '/';

    const auto position = path.find_last_of(separator);
    const bool isPath = position != std::string::npos;

    return isPath ? path.substr(position + 1) : path;
}

}