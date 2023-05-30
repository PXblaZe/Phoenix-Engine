#pragma once

namespace px {

    class VertexArray {

        unsigned int vaID;

    public:

        VertexArray();

        void bind() const;

        void unbind() const;

    };

} // namespace p 