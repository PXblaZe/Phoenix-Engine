#pragma once
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace px
{

class Viewspace {
    
    glm::vec3 position, front, up, right, rotation, worldUp;

    public:

    float turn_speed = 0.1f;
    glm::vec3 movement_velocity = glm::vec3(1.f);


    Viewspace(const glm::vec3& position, const glm::vec3& rotation = glm::vec3(0.f), const glm::vec3& worldUp = {0, 1, 0});

    void updateOrientation();

    inline void setPosition(const glm::vec3& value) noexcept { position = value; }

    inline void setPosition(float x, float y, float z) noexcept { position = glm::vec3(x, y, z); }

    inline void setFrontVector(const glm::vec3& value) noexcept { front = value; }

    inline void setFrontVector(float fx, float fy, float fz) noexcept { front = glm::vec3(fx, fy, fz); }

    inline void setUpVector(const glm::vec3& value) noexcept { up = value; }

    inline void setUpVector(float ux, float uy, float uz) noexcept { up = glm::vec3(ux, uy, uz); }

    inline void setPitch(float radians) noexcept { rotation.x = radians; }

    inline void setYaw(float radians) noexcept { rotation.y = radians; }

    inline void setRoll(float radians) noexcept { rotation.z = radians; }

    const glm::mat4 getViewMatrix() const noexcept;

    inline const glm::vec3& getPosition() const noexcept { return position; }

    inline const glm::vec3& getFrontVector() const noexcept { return front; }

    inline const glm::vec3& getUpVector() const noexcept { return up; }

    inline const glm::vec3& getRightVector() const noexcept { return right; }

    inline const float& getPitch() const noexcept { return rotation.x; }

    inline const float& getYaw() const noexcept { return rotation.y; } 

    inline const float& getRoll() const noexcept { return rotation.z; }

    void mouseControl(double xChange, double yChange);

};

} // namespace px
