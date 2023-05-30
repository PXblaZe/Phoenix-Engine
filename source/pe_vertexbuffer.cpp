#include "PXE/pe_vertexbuffer.hpp"

#include <iostream>

namespace px {

    VertexBuffer::VertexBuffer(size_t size,const void* data, unsigned int glUsage):bufferSize(size) {
        PXcall(glGenBuffers(1, &vboID));
        PXcall(glBindBuffer(GL_ARRAY_BUFFER, vboID));
        PXcall(glBufferData(GL_ARRAY_BUFFER, size, data, glUsage));
    }

    void VertexBuffer::updateBufferData(const void* data, unsigned int offset, size_t size) const {
        PXcall(glBufferSubData(GL_ARRAY_BUFFER, offset, size==NULL ? bufferSize-offset : size, data));
    }

    void VertexBuffer::getBufferData(void *data, unsigned int offset, size_t size) const {
        PXcall(glGetBufferSubData(GL_ARRAY_BUFFER, offset, size==NULL ? bufferSize-offset : size, data));
    }

    void VertexBuffer::bind() const {
        PXcall(glBindBuffer(GL_ARRAY_BUFFER, vboID));
    }

    void VertexBuffer::unbind() const {
        PXcall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    }

} // namespace px
