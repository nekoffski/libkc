#pragma once

#include "Deserializable.h"
#include "Serializable.h"

namespace kc::model {

struct Model : virtual public Deserializable, virtual public Serializable { };

}