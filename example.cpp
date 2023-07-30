#include <iostream>

#define PX_DEBUG

#include "PXE/pe_window.hpp"
#include "PXE/pe_program.hpp"
#include "PXE/pe_vertexarray.hpp"

void rcb(const px::VertexArray& vao, const px::ElementBuffer& ibo) {
    vao.bind(), ibo.bind();
    glDrawElements(GL_TRIANGLES, ibo.elementCount(), ibo.draw_callType(), nullptr);
    vao.unbind(), ibo.unbind();
}

int main() {
    
    px::Window win(800, 600, "Phoenix Engine");
    // px::Window win("While Noise");

    GLfloat v[8] {
        -.5, .5,
        .5, .5,
        .5, -.5,
        -.5, -.5
    };

    px::VertexArray vao;
    px::VertexBuffer vbo(4*8, v);
    px::VertexBufferLayout layout;
    layout.push<float>(2);
    vao.attachVertexBuffer(vbo, layout);

    // GLbyte i[6] {0, 1, 2, 2, 3, 0};

    // px::ElementBuffer ibo(6, i);
    px::ElementBuffer ibo((char)0, 1, 2, 2, 3, 0);
    std::cout << std::hex << ibo.draw_callType() << std::endl;

    px::ShaderCode sc("testShader.glsl");
    px::ShaderProgram sp;
    sp.createProgram(sc.parseCode("ver"), sc.parseCode("frag"));
    px::ShaderProgram::use(sp);

    vao.unbind(), ibo.unbind();

    // // std::cout << sc.parseCode("ver") << std::endl << sc.parseCode("frag") << std::endl;
    // std::cout << "vsID: " << sp.getShaderID(px::ShaderProgram::VERTEX_SHADER) << std::endl;
    // std::cout << "fsID: " << sp.getShaderID(px::ShaderProgram::FRAGMENT_SHADER) << std::endl;
    sp.cleanShaders(px::ShaderProgram::FRAGMENT_SHADER | px::ShaderProgram::VERTEX_SHADER);
    // std::cout << "vsID: " << sp.getShaderID(px::ShaderProgram::VERTEX_SHADER) << std::endl;
    // std::cout << "fsID: " << sp.getShaderID(px::ShaderProgram::FRAGMENT_SHADER) << std::endl;
    win.run<const px::VertexArray&, const px::ElementBuffer&>(rcb, vao, ibo);

    return 0;
}