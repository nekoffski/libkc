#pragma once

#include <typeindex>
#include <memory>

namespace kc::event2 {

struct EventStorageBase {
    virtual ~EventStorageBase()                  = default;
    virtual std::type_index getTypeIndex() const = 0;
    virtual void* getData()                      = 0;

    template <typename T> T* as() {
        if (const auto typeIndex = std::type_index(typeid(T)); typeIndex != getTypeIndex())
            return nullptr;
        return static_cast<T*>(getData());
    }
};

template <typename T> class EventStorage : public EventStorageBase {
   public:
    template <typename... Args>
    explicit EventStorage(Args&&... args) : m_data(std::forward<Args>(args)...) {}

    std::type_index getTypeIndex() const override { return typeid(T); }

    void* getData() override { return static_cast<void*>(&m_data); }

   private:
    T m_data;
};

}  // namespace kc::event2
