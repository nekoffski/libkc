#pragma once

#include "Category.h"
#include "Event.h"
#include "fwd.h"
#include "kc/core/Log.h"

namespace kc::event {

namespace detail {
using TypeVector = std::vector<std::type_index>;

template <typename Category, typename... Rest>
struct Extractor {
    static void extract(TypeVector& categories) {
        categories.emplace_back(typeid(Category));
        Extractor<Rest...>::extract(categories);
    }
};

template <typename Category>
struct Extractor<Category> {
    static void extract(TypeVector& categories) { categories.emplace_back(typeid(Category)); }
};
}  // namespace detail

class EventProvider {
   public:
    explicit EventProvider(const CategoryToEventQueue& categoryToEventQueue);

    EventQueue getAll() const;

    template <CategoryType... Categories>
    EventQueue getByCategories() const {
        detail::TypeVector categories;
        detail::Extractor<Categories...>::extract(categories);

        EventQueue events;
        for (auto categoryIndex : categories) {
            if (not m_categoryToEventQueue.contains(categoryIndex)) continue;

            auto& queue = m_categoryToEventQueue[categoryIndex];
            events.insert(events.begin(), queue.begin(), queue.end());
        }

        if (auto n = events.size(); n > 0) LOG_TRACE("Events: {}", n);

        return events;
    }

   private:
    mutable CategoryToEventQueue m_categoryToEventQueue;
};
}  // namespace kc::event
