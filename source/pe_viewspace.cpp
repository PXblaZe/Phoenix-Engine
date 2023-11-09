#include <glm/trigonometric.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "PXE/pe_events.hpp"
#include "PXE/pe_viewspace.hpp"

namespace px
{
    
    Viewspace::Viewspace(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& worldUp)
    : position(position), worldUp(worldUp), rotation(rotation) {
        updateOrientation();
        
    }

    const glm::mat4 Viewspace::getViewMatrix() const noexcept
    {
        return glm::lookAt(position, position+front, up);
    }

    void Viewspace::updateOrientation()
    {
        float roll = rotation.z, pitch = rotation.x, yaw = rotation.y;

        // Calculate new front vector
        glm::vec3 newFront;
        newFront.x = glm::cos(yaw) * glm::cos(pitch);
        newFront.y = glm::sin(pitch);
        newFront.z = glm::sin(yaw) * glm::cos(pitch);
        front = glm::normalize(newFront);

        // Calculate new right and up vectors
        right = glm::normalize(glm::cross(front, worldUp));
        up = glm::normalize(glm::cross(right, front));

        // Precalculate roll rotation matrix elements
        float cosRoll = glm::cos(roll);
        float sinRoll = glm::sin(roll);

        // Apply roll rotation to up and right vectors
        glm::vec3 newUp = up * cosRoll - right * sinRoll;
        glm::vec3 newRight = up * sinRoll + right * cosRoll;

        up = newUp;
        right = newRight;
    }

    void Viewspace::mouseControl(double xChange, double yChange)
    {    
        
        xChange *= turn_speed;
        yChange *= turn_speed;

        rotation.y += glm::radians(xChange);
        rotation.x += glm::radians(yChange);

        if(glm::degrees(rotation.x) > 89.0f) rotation.x = glm::radians(89.0f);
        if(glm::degrees(rotation.x) < -89.0f) rotation.x = glm::radians(-89.0f);

        this->updateOrientation();
    }


} // namespace px
 