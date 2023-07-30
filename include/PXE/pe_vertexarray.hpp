#pragma once

#include "PXE/pe_buffers.hpp"
#include "PXE/pe_vertexbufferlayout.hpp"

namespace px {

    class VertexArray {

        unsigned int vaoID;

    public:

        VertexArray();

        ~VertexArray();

        void attachVertexBuffer(const VertexBuffer& vb, const VertexBufferLayout& vblayout) const;
        
        void bind() const;

        static void unbind();

    };

} // namespace p 