#pragma once

#include <kc/core/Log.h>
#include <stb.h>
#include <stb_write.h>

#include <glm/glm.hpp>
#include <iostream>

#include "kc/core/FileSystem.h"

namespace kc::cv {

class Image {
   public:
    struct Size {
        int width;
        int height;
    };

    explicit Image(Size&& size) : m_size(std::move(size)), m_data(nullptr) {
        const int s = m_size.width * m_size.height * m_channels;
        std::cout << "Allocating: " << s << '\n';
        m_data = new unsigned char[s];
        for (int i = 0; i < s; ++i) m_data[i] = 0;
    }

    ~Image() { delete m_data; }

    const Size getSize() const { return m_size; }

    void setPixelValue(int i, int j, const glm::vec3 color) {
        const int startIndex = (i * m_size.width + j) * m_channels;

        ASSERT(
            (startIndex + 2) <= (m_size.height * m_size.width * m_channels),
            "Index out of image size"
        );

        for (int k = 0; k < m_channels; ++k) m_data[startIndex + k] = color[k] * 255;
    }

    void save(const std::string& path) {
        constexpr int quality = 100;
        stbi_write_jpg(path.c_str(), m_size.width, m_size.height, m_channels, m_data, quality);
    }

   private:
    Size m_size;
    unsigned char* m_data;

    int m_channels = 3;
};

}  // namespace kc::cv