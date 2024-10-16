#pragma once

#define CONCATENATE_IMPL(s1, s2) s1##s2
#define CONCATENATE(s1, s2) CONCATENATE_IMPL(s1, s2)

#ifdef __COUNTER__
#define ANONYMOUS_VAR(str) CONCATENATE(str, __COUNTER__)
#else
#define ANONYMOUS_VAR(str) CONCATENATE(str, __LINE__)
#endif

#define KC_STATIC_CLASS(T)           \
public:                              \
    T()                    = delete; \
    T(const T&)            = delete; \
    T(const T&&)           = delete; \
    T& operator=(const T&) = delete; \
    T& operator=(T&&)      = delete;

#define REPEAT(x) for (unsigned int __i = 0; __i < x; ++__i)
#define LOOP for (;;)

#define MOVE_THIS return std::move(*this)

// clang-format off
#define EMPTY_METHOD {}
#define UNREACHABLE_CODE __builtin_unreachable()
// clang-format on

#define TO_STRING_STREAM(Type)                                                 \
    inline std::ostream& operator<<(std::ostream& stream, const Type& value) { \
        stream << toString(value);                                             \
        return stream;                                                         \
    }
