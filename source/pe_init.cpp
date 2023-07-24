#include <sstream>
#include <string.h>
#include <iostream>

#include "PXE/pe_init.hpp"

const char* to_hex(unsigned int num)
{
    std::stringstream ss;
    ss << std::hex << num;
    return strcpy((char*)malloc(9), ss.str().c_str());
}

void PXclearGLERlogs()
{
    while (glGetError());
}

bool PXdisGLERlog(const char* funname, const char* filename, size_t line) {
    while (GLenum err = glGetError()) {
        std::cout << "[OpenGL Error: " << line << "] (0x" << std::hex << err << ") " << funname << ' ' << filename << std::endl; 
        return false;
    }
    return true;
}