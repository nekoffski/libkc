#pragma once

#include <string>
#include <typeindex>

namespace kc::model {

class Deserializable {
   public:
    virtual std::string getName() const = 0;

    template <typename T>
    bool is() const {
        return typeid(T) == getTypeIndex();
    }

    template <typename T>
    const T* as() const {
        return dynamic_cast<const T*>(this);
    }

   private:
    virtual std::type_index getTypeIndex() const = 0;
};

template <typename T>
struct DeserializableBase : public Deserializable {
    std::type_index getTypeIndex() const override { return typeid(T); }
};

}  // namespace kc::model