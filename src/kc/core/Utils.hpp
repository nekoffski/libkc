#pragma once

namespace kc::core {

template <typename T, typename Allocator, template <typename, typename> typename Container>
int indexOf(const Container<T, Allocator>& container, const T& value,
            const int notFoundIndex = -1) {
    using SizeType = typename Container<T, Allocator>::size_type;

    for (SizeType i = 0; i < container.size(); ++i)
        if (container[i] == value) return i;

    return notFoundIndex;
}

}  // namespace kc::core