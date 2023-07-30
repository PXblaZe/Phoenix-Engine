#include "PXE/pe_init.hpp"
#include "PXE/pe_vertexbufferlayout.hpp"

namespace px {

    VertexBufferLayout::VertexBufferLayout(): stride(0) {}

    void VertexBufferLayout::pushf(unsigned int count, bool normalized)
    {
        this->insert(
            GLtype<float>::enumval, count, sizeof(GLtype<float>::type),
            (unsigned char)(normalized ? GL_TRUE : GL_FALSE)
        );
        this->stride += sizeof(GLtype<float>::type)*count;
    }

    void VertexBufferLayout::pushd(unsigned int count, bool normalized)
    {
        this->insert(
            GLtype<double>::enumval, count, sizeof(GLtype<double>::type),
            (unsigned char)(normalized ? GL_TRUE : GL_FALSE)
        );
        this->stride += sizeof(GLtype<double>::type)*count;
    }

    void VertexBufferLayout::pushb(unsigned int count, bool normalized)
    {
        this->insert(
            GLtype<signed char>::enumval, count, sizeof(GLtype<signed char>::type),
            (unsigned char)(normalized ? GL_TRUE : GL_FALSE)
        );
        this->stride += sizeof(GLtype<signed char>::type)*count;
    }

    void VertexBufferLayout::pushub(unsigned int count, bool normalized)
    {
        this->insert(
            GLtype<unsigned char>::enumval, count, sizeof(GLtype<unsigned char>::type),
            (unsigned char)(normalized ? GL_TRUE : GL_FALSE)
        );
        this->stride += sizeof(GLtype<unsigned char>::type)*count;
    }

    void VertexBufferLayout::pushi(unsigned int count, bool normalized)
    {
        this->insert(
            GLtype<signed int>::enumval, count, sizeof(GLtype<signed int>::type),
            (unsigned char)(normalized ? GL_TRUE : GL_FALSE)
        );
        this->stride += sizeof(GLtype<signed int>::type)*count;
    }

    void VertexBufferLayout::pushui(unsigned int count, bool normalized)
    {
        this->insert(
            GLtype<unsigned int>::enumval, count, sizeof(GLtype<unsigned int>::type),
            (unsigned char)(normalized ? GL_TRUE : GL_FALSE)
        );
        this->stride += sizeof(GLtype<unsigned int>::type)*count;
    }

    void VertexBufferLayout::pushs(unsigned int count, bool normalized)
    {
        this->insert(
            GLtype<signed short>::enumval, count, sizeof(GLtype<signed short>::type),
            (unsigned char)(normalized ? GL_TRUE : GL_FALSE)
        );
        this->stride += sizeof(GLtype<signed short>::type)*count;
    }

    void VertexBufferLayout::pushus(unsigned int count, bool normalized)
    {
        this->insert(
            GLtype<unsigned short>::enumval, count, sizeof(GLtype<unsigned short>::type),
            (unsigned char)(normalized ? GL_TRUE : GL_FALSE)
        );
        this->stride += sizeof(GLtype<unsigned short>::type)*count;
    }

} // namespace px 
