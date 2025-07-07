#pragma once

#include <vector>
#include <stdio.h>
#include <algorithm>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>


namespace px
{
    
class Mesh  {

    unsigned char vd, td, nd;
    glm::mat4 model_mat;
    std::vector<double> dmodel_data;
    std::vector<float> fmodel_data;
    
    void __from_wavefront(FILE* pOBJ);
    void __from_wavefront_v2(FILE* pOBJ) noexcept;


public:

    Mesh();
    Mesh(const char* filename);
    Mesh(size_t size, void* data);

    void loadModel(const char* filename, double*, double*);

    void scale(double vx, double vy, double vz);
    void rotate(double angle, double ax, double ay, double az);
    void translate(double px, double py, double pz);
    inline constexpr const glm::vec3 getPosition() const noexcept
    {
        return model_mat[3];
    }

    inline constexpr const glm::mat4 getModelMatrix() const noexcept
    {
        return model_mat;
    }
    
    template<typename _RT = double, typename = std::enable_if_t<std::__is_one_of<_RT, signed int, unsigned int, float, double>::value>>
    inline constexpr const _RT* getModel() noexcept
    {
        if constexpr (std::is_same<_RT, double>::value) return dmodel_data.data();
        else if constexpr (std::is_same<_RT, float>::value) {
            if (!fmodel_data.empty()) return fmodel_data.data();
            fmodel_data.reserve(dmodel_data.size());
            std::transform(
                dmodel_data.begin(), dmodel_data.end(),
                fmodel_data.begin(), [](double x) -> float { return float(x); }
            );
            return fmodel_data.data();
        }
        else if constexpr (std::is_same<_RT, signed int>::value) {

        }
    }

};

} // namespace px
