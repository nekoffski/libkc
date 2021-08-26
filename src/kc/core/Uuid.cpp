#include "Uuid.h"

#include <crossguid/guid.hpp>

namespace kc::core {

Uuid generateUuid() {
    return xg::newGuid().str();
}
}