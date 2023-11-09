#pragma once

#include <vector>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace px
{
    
class Mesh  {

    unsigned char vd, td, nd;
    glm::mat4 model_mat;
    std::vector<double> model_data;
    
    void __from_wavefront(FILE* pOBJ);

public:

    Mesh();
    Mesh(const char* filename);
    Mesh(size_t size, void* data);
    ~Mesh();

    void loadModel(const char* filename);

    void scale(double vx, double vy, double vz);
    void rotate(double angle, double ax, double ay, double az);
    void translate(double px, double py, double pz);

    inline constexpr const glm::mat4 getModelMatrix() const noexcept
    {
        return model_mat;
    }
    
    template<typename _RT = double>
    const _RT* getModel() const noexcept
    {
        return static_cast<const _RT*>(model_data.data());
    }

};

} // namespace px
