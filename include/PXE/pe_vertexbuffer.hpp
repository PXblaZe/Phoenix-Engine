#pragma once

#include "PXE/pe_init.hpp"

namespace px {

    class VertexBuffer {

        unsigned int vboID;

    public:

        const unsigned int bufferSize;

        VertexBuffer(size_t size, const void* data = nullptr, unsigned int glUsage = GL_STATIC_DRAW);

        void updateBufferData(const void* data, unsigned int offset = 0, size_t size = NULL) const;

        void getBufferData(void* data, unsigned int offset = 0, size_t size = NULL) const;

        void bind() const;

        void unbind() const;

    };

}