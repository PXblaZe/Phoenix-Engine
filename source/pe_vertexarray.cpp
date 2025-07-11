
#include "PXE/pe_init.hpp"
#include "PXE/pe_vertexarray.hpp"


namespace px
{
    
    VertexArray::VertexArray() 
    {
    #if defined(GL_VERSION_3_0) || defined(GL_ES_VERSION_3_0)
        glGenVertexArrays(1, &vaoID);
        glBindVertexArray(vaoID);
    #else
        this->vaoID = 0;
    #endif
    }

    VertexArray::~VertexArray() { glDeleteVertexArrays(1, &vaoID); }

    void VertexArray::attachVertexBuffer(const VertexBuffer& vb, const VertexBufferLayout& vblayout) const
    {
        this->bind();
        vb.bind();
        const auto& layout = vblayout();
        unsigned int offset = 0;
        for (unsigned int i = 0; i < layout.size(); i++)
        {
            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i, layout[i].count, layout[i].type, layout[i].normalized, vblayout.getStride(), (const void*)offset);
            offset += layout[i].count * layout[i].type_size;
        }
    }

    void VertexArray::bind() const { glBindVertexArray(vaoID); }

    void VertexArray::unbind() { glBindVertexArray(0); }

} // namespace px
