#pragma once

#include "Deserializable.h"
#include "Serializable.h"
#include "kc/core/ErrorBase.hpp"

DEFINE_ERROR(ModelError);

namespace kc::model {

template <typename T>
struct Model : virtual public DeserializableBase<T>, virtual public Serializable {};

}  // namespace kc::model