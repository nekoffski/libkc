#pragma once

#include "Deserializable.h"
#include "Serializable.h"

namespace kc::model {

template <typename T>
struct Model : virtual public Deserializable<T>, virtual public Serializable { };

}