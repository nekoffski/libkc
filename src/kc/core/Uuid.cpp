#include "Uuid.h"

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace kc::core {

Uuid generateUuid() {
    using namespace boost;
    return lexical_cast<Uuid>(uuids::random_generator()());
}
}