#pragma once

#include <vector>

namespace kc::core {

template <typename T>
class Countable {
public:
    Countable()
        : m_id(generateId()) {
    }

    ~Countable() {
        freeIds.push_back(m_id);
    }

    Countable(const Countable&) = delete;
    Countable& operator=(const Countable&) = delete;

    unsigned int getId() {
        return m_id;
    }

private:
    unsigned int generateId() const {
        if (not freeIds.empty()) {
            auto freeId = freeIds.back();
            freeIds.pop_back();

            return freeId;
        }

        return currentId++;
    }

    unsigned int m_id;

    inline static std::vector<unsigned int> freeIds;
    inline static unsigned int currentId = 0;
};

}