#include <iostream>

#define PX_DEBUG

#include "PXE/pe_window.hpp"
#include "PXE/pe_vertexbuffer.hpp"

void rcb(int a , float b, const char* str) {
    
}

int main() {

    // px::Window win(1, 1, "Phoenix Engine");
    px::Window win("While Noise");

    win.run<int, float, const char*>(rcb, 10, 3.14, "Vipul");

    return 0;
}