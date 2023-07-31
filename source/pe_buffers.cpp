

#include "PXE/pe_init.hpp"
#include "PXE/pe_buffers.hpp"

namespace px {

    VertexBuffer::VertexBuffer(unsigned int size,const void* data): bufferSize(size)
    {
        PXcall(glGenBuffers(1, &vboID));
        PXcall(glBindBuffer(GL_ARRAY_BUFFER, vboID));
        PXcall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
    }
    
    VertexBuffer::VertexBuffer(unsigned int size, unsigned int glUsage, const void* data): bufferSize(size)
    {
        PXcall(glGenBuffers(1, &vboID));
        PXcall(glBindBuffer(GL_ARRAY_BUFFER, vboID));
        PXcall(glBufferData(GL_ARRAY_BUFFER, size, data, glUsage));
    }

    VertexBuffer::~VertexBuffer() {
        PXcall(glDeleteBuffers(1, &vboID));
    }

    void VertexBuffer::updateBufferData(const void *data, unsigned int offset, size_t size) const {
        PXcall(glBufferSubData(GL_ARRAY_BUFFER, offset, size ? size : bufferSize-offset, data));
    }

    void VertexBuffer::getBufferData(void *data, unsigned int offset, size_t size) const {
        PXcall(glGetBufferSubData(GL_ARRAY_BUFFER, offset, size ? size : bufferSize-offset, data));
    }

    void VertexBuffer::bind() const {
        PXcall(glBindBuffer(GL_ARRAY_BUFFER, vboID));
    }

    void VertexBuffer::unbind() {
        PXcall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    }


    
    ElementBuffer::~ElementBuffer() {
        PXcall(glDeleteBuffers(1, &iboID));
    }

    void ElementBuffer::create(unsigned int type_size, unsigned int count, const void* data)
    {
        this->count = count;
        this->type_size = type_size;
        PXcall(glGenBuffers(1, &iboID));
        PXcall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID));
        PXcall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, type_size*count, data, GL_STATIC_DRAW));
    }

    void ElementBuffer::updateBufferData(const void *data, unsigned int offset, size_t size) const {
        PXcall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size ? size : type_size*count-offset, data));
    }

    void ElementBuffer::getBufferData(void *data, unsigned int offset, size_t size) const {
        PXcall(glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size ? size : type_size*count-offset, data));
    }

    const unsigned int ElementBuffer::draw_callType() const  noexcept
    {
        switch(type_size)
        {
            case sizeof(GLubyte): return GL_UNSIGNED_BYTE;
            case sizeof(GLushort): return GL_UNSIGNED_SHORT;
            case sizeof(GLuint): return GL_UNSIGNED_INT;
        }
        return 0;
    }

    const unsigned int ElementBuffer::elementCount() const noexcept { return count; }

    void ElementBuffer::bind() const {
        PXcall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID));
    }

    void ElementBuffer::unbind() {
        PXcall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    }

} // namespace px
