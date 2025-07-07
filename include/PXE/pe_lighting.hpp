#pragma once

#include <glm/vec3.hpp>

#include "PXE/pe_program.hpp"

namespace px
{

class PhongLighting {
    glm::vec3 color;
    float intensity;
public:

    PhongLighting();
    virtual ~PhongLighting() {}
    PhongLighting(glm::vec3 color, float intensity);
    void useAmbientLight(const ShaderProgram& shader, const char* colorUniformName, const char* intensityUniformName) const;
    void setColor(glm::vec3 newColor);
    void setIntensity(float);
    glm::vec3 getColor() const;
    float getIntensity() const;
    void useLight(const ShaderProgram& shader, const char* colorUniformName, const char* intensityUniformName) const;
};


} // namespace px
