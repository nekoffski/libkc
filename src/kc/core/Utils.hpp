#pragma once

#include <algorithm>
#include <cstring>
#include <sstream>

namespace kc::core {

template <typename T, typename Allocator, template <typename, typename> typename Container>
int indexOf(const Container<T, Allocator>& container, const T& value,
            const int notFoundIndex = -1) {
    using SizeType = typename Container<T, Allocator>::size_type;

    for (SizeType i = 0; i < container.size(); ++i)
        if (container[i] == value) return i;

    return notFoundIndex;
}

template <typename T, typename Allocator, template <typename, typename> typename Container>
bool contains(const Container<T, Allocator>& container, const T& value) {
    return std::find(container.begin(), container.end(), value) != container.end();
}

template <typename Allocator, template <typename, typename> typename Container>
bool contains(const Container<const char*, Allocator>& container, const char* const& value) {
    auto comparator = [value](const char* oth) { return std::strcmp(oth, value) == 0; };
    return std::find_if(container.begin(), container.end(), comparator) != container.end();
}

template <typename T, typename Allocator, template <typename, typename> typename Container>
std::string join(const Container<T, Allocator>& container, const std::string& separator) {
    std::stringstream stream;

    for (const auto& value : container) stream << value << separator;

    auto output = stream.str();
    return output.substr(0, output.size() - separator.size());
}

}  // namespace kc::core
