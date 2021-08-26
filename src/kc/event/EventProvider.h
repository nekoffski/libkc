#pragma once

#include "Category.h"
#include "Event.h"
#include "fwd.h"

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
        static void extract(TypeVector& categories) {
            categories.emplace_back(typeid(Category));
        }
    };
}

class EventProvider {
public:
    explicit EventProvider(CategoryToEventQueue& categoryToEventQueue);

    EventQueue getAll() const;

    template <CategoryType... Categories>
    EventQueue getByCategories() const {
        detail::TypeVector categories;
        detail::Extractor<Categories...>::extract(categories);

        EventQueue events;
        for (const auto& categoryIndex : categories) {
            if (not m_categoryToEventQueue.contains(categoryIndex))
                continue;

            auto& queue = m_categoryToEventQueue[categoryIndex];
            auto lock = queue.lock();
            events.insert(events.begin(), queue.begin(), queue.end());
            queue.clearUnsafe();
        }

        return events;
    }

private:
    CategoryToEventQueue& m_categoryToEventQueue;
};
}
