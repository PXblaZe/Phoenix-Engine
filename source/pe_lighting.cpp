#include "PXE/pe_lighting.hpp"

namespace px 
{
    PhongLighting::PhongLighting(): color(glm::vec3(1.f)), intensity(1) {}
    PhongLighting::PhongLighting(glm::vec3 color, float intensity): color(color), intensity(intensity) {}

    void PhongLighting::setColor(glm::vec3 newColor) {color = newColor;}
    void PhongLighting::setIntensity(float newIntensity) {intensity = newIntensity;}

    glm::vec3 PhongLighting::getColor() const {return color;}
    float PhongLighting::getIntensity() const {return intensity;}

    void PhongLighting::useLight(const ShaderProgram& shader, const char* colorUniformName, const char* intensityUniformName) const
    {
        glm::vec3 color = getColor();
        shader.setUniform<3>(colorUniformName, color.r, color.g, color.b);
        shader.setUniform(intensityUniformName, getIntensity());
    }

}
