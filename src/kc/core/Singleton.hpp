#pragma once

#include <memory>

#include "Log.h"

namespace kc::core {

template <typename T>
class Singleton {
public:
    explicit Singleton() {
        ASSERT(s_instance == nullptr, "Only 1 instance of singleton is allowed");
        s_instance = (T*)this;
    }

    virtual ~Singleton() {
        s_instance = nullptr;
    }

    static T& get() {
        return *s_instance;
    }

    static T* getPtr() {
        return s_instance;
    }

    Singleton(const Singleton& oth) = delete;
    Singleton(Singleton&& oth) = delete;

    Singleton& operator=(const Singleton& oth) = delete;
    Singleton& operator=(Singleton&& oth) = delete;

private:
    inline static T* s_instance = nullptr;
};

}