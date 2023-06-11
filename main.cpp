#include <iostream>

#define PX_DEBUG

#include "PXE/pe_window.hpp"
#include "PXE/pe_vertexbuffer.hpp"

void rcb(int a , float b, const char* str) {

}

int main() {

    // px::Window win(1, 1, "Phoenix Engine");
    px::Window win("PX Engine");
    // win.showWindow();
    
    px::VertexBuffer vb(4*50);
    void* p = malloc(4*10);
    vb.updateBufferData(p);
    win.run<int, float, const char*>(rcb, 10, 3.14, "Vipul");

    return 0;
}