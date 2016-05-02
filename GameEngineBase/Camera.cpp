//
// Created by Glenn Jacob on 29/12/15.
//

#include "ShaderProgram.h"
#include "MasterRenderer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"

namespace ForlornMelody {
namespace GameEngine {
namespace Base {

Camera::Camera()
{
}

Camera::~Camera()
{
}

vec3 Camera::getPosition() const
{
    return this->position;
}

float Camera::getPitch() const
{
    return this->pitch;
}

float Camera::getYaw() const
{
    return this->yaw;
}

float Camera::getRoll() const
{
    return this->roll;
}

void Camera::move(Camera::Direction direction, float speed)
{
    switch (direction) {
        case LEFT:
            this->position.x -= speed;
            break;

        case RIGHT:
            this->position.x += speed;
            break;

        case FORWARD:
            this->position.z -= speed;
            break;

        case BACKWARD:
            this->position.z += speed;
            break;

        case DOWN:
            this->position.y -= speed;
            break;

        case UP:
            this->position.y += speed;
            break;

        default:
            break;
    }
}

mat4 Camera::createViewMatrix() const
{
    auto viewMatrix = mat4(1.0f); // identity matrix
    viewMatrix = glm::rotate(viewMatrix, glm::radians(this->getPitch()), vec3(1.0f, 0.0f, 0.0f));
    viewMatrix = glm::rotate(viewMatrix, glm::radians(this->getYaw()), vec3(0.0f, 1.0f, 0.0f));
    viewMatrix = glm::rotate(viewMatrix, glm::radians(this->getRoll()), vec3(0.0f, 0.0f, 1.0f));

    auto cameraPos = this->getPosition();
    viewMatrix = glm::translate(viewMatrix, -cameraPos);

    return viewMatrix;
}

void Camera::setPosition(const glm::vec3& position)
{
    this->position = position;
}

void Camera::setX(const float x)
{
    position.x = x;
}

void Camera::setY(const float y)
{
    position.y = y;
}

void Camera::setZ(const float z)
{
    position.z = z;
}
}
}
}



