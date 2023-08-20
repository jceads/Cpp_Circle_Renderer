#include "Camera.h"

Camera::Camera(float     fov,
               float     nearPlane,
               float     farPlane,
               glm::vec3 position,
               glm::vec3 lookAt): fov(fov),
                                  nearPlane(nearPlane),
                                  position(position),
                                  up(up),
                                  farPlane(farPlane),
                                  lookAt(lookAt)
{
}

void Camera::ChangePosition(glm::vec3 newPosition)
{
    position = newPosition;
}
