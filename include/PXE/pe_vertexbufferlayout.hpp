#pragma once


namespace px {

    class VertexBufferLayout {

        struct VertexArrayAttrib {
            unsigned int type;
            unsigned int count;
            unsigned char normalized;

            static unsigned int getStride(unsigned int GLtype); 

        };

    unsigned int VertexArrayAttrib(unsigned int GLtype);

};

};