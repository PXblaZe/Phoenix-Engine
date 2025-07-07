#include "PXE/pe_mesh.hpp"

#include <iostream>


signed main() {
    px::Scene Scene;
    Scene.elements.emplace_back("models/TrialFreya_OBJ/TrialFreya_OBJ.obj");
    std::cout << "Last\n";
    // for (const px::Object& o : Scene.elements) o.renderObject();
}
