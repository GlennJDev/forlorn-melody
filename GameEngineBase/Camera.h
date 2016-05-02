//
// Created by Glenn Jacob on 29/12/15.
//

#ifndef MUSICBOX_CAMERA_H
#define MUSICBOX_CAMERA_H
#include <glm/glm.hpp>

using glm::vec3;
using glm::mat4;

namespace ForlornMelody {
namespace GameEngine {
namespace Base {

class Camera
{
public:

    enum Direction
    {
        LEFT, RIGHT, FORWARD, BACKWARD, UP, DOWN
    };

    Camera();
    virtual ~Camera();

    vec3 getPosition() const;
    float getPitch() const;
    float getYaw() const;
    float getRoll() const;

    void move(Direction direction, float speed);
    void setPosition(const glm::vec3& position);
    void setX(const float x);
    void setY(const float y);
    void setZ(const float z);
    mat4 createViewMatrix() const;

protected:
    vec3 position{0.0f};
    float pitch = 0.0f;
    float yaw = 0.0f;
    float roll = 0.0f;
};
} // Custom
} // GameEngine
} // ForlornMelody



#endif //MUSICBOX_CAMERA_H
