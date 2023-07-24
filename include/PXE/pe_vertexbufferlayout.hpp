#pragma once

#include <vector>
namespace px {

class VertexBufferLayout {

    struct VertexArrayAttrib {
        unsigned int type;
        unsigned int count;
        unsigned char normalized;
        static unsigned int getStride(unsigned int GLtype); 
    };

    std::vector<struct VertexBufferLayout::VertexArrayAttrib> layout;

public:
    template<typename T>
    void attachAttrib(bool normalized = false)
    {
        layout.push_back({
            GLtype<T>::enumval, 
            layout.size(), 
            normalized ? 1 : 0
        });
    }

};

} // namespace px