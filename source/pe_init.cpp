#include <iostream>

#include "PXE/pe_init.hpp"

void PXclearGLERlogs() {
    while (glGetError());
}

bool PXdisGLERlog(const char* funname, const char* filename, size_t line) {
    while (GLenum err = glGetError()) {
        std::cout << "[OpenGL Error: " << line << "] (0x" << std::hex << err << ") " << funname << ' ' << filename << std::endl; 
        return false;
    }
    return true;
}