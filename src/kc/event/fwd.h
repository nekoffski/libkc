#pragma once

#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include "Containers.hpp"

namespace kc::event {

struct Event;
class EventListener;
class EventEmitter;

using EventListeners = AtomicUnorderedMultimap<std::string, EventListener*>;
using EventQueue = AtomicVector<std::shared_ptr<Event>>;
using CategoryToEventQueue = AtomicUnorderedMap<std::type_index, EventQueue>;
using EventContainer = AtomicUnorderedMap<std::string, CategoryToEventQueue>;
};
