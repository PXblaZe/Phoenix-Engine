#include <iostream>

#define PX_DEBUG

#include "PXE/pe_window.hpp"
#include "PXE/pe_program.hpp"
#include "PXE/pe_vertexbuffer.hpp"

void rcb(int a , float b, const char* str) {
    
}

int main() {

    px::Window win(800, 600, "Phoenix Engine");
    // px::Window win("While Noise");

    px::ShaderCode sc(".test/testShader.glsl");
    px::ShaderProgram sp;
    sp.createProgram(sc.parseCode("ver"), sc.parseCode("frag"));
    px::ShaderProgram::use(sp);

    // std::cout << sc.parseCode("ver") << std::endl << sc.parseCode("frag") << std::endl;
    std::cout << "vsID: " << sp.getShaderID(px::ShaderProgram::VERTEX_SHADER) << std::endl;
    std::cout << "fsID: " << sp.getShaderID(px::ShaderProgram::FRAGMENT_SHADER) << std::endl;
    sp.cleanShaders(px::ShaderProgram::FRAGMENT_SHADER | px::ShaderProgram::VERTEX_SHADER);
    std::cout << "vsID: " << sp.getShaderID(px::ShaderProgram::VERTEX_SHADER) << std::endl;
    std::cout << "fsID: " << sp.getShaderID(px::ShaderProgram::FRAGMENT_SHADER) << std::endl;
    win.run<int, float, const char*>(rcb, 10, 3.14, "Vipul");

    return 0;
}