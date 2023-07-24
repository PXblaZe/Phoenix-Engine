#pragma once

namespace px {

    class VertexBuffer {

        unsigned int vboID;

    public:

        const unsigned int bufferSize;

        VertexBuffer(size_t size, const void* data = nullptr);
        VertexBuffer(size_t size, unsigned int glUsage, const void* data = nullptr);
        ~VertexBuffer();

        void updateBufferData(const void* data, unsigned int offset = 0, size_t size = 0) const;

        void getBufferData(void* data, unsigned int offset = 0, size_t size = 0) const;

        void bind() const;

        void unbind() const;

    };

}