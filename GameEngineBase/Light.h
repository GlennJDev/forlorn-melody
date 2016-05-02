//
// Created by Glenn Jacob on 12/03/16.
//

#ifndef MUSICBOX_LIGHT_H
#define MUSICBOX_LIGHT_H

#include <glm/glm.hpp>

using glm::vec3;

namespace ForlornMelody {
namespace GameEngine {
namespace Base {

class Light
{
public:
    Light();
    Light(const vec3& position, const vec3& color);
    virtual ~Light();

    const vec3& getPosition() const;
    void setPosition(const vec3& position);
    const vec3& getColor() const;
    void setColor(const vec3& color);

private:
    vec3 position;
    vec3 color;
};
} // Base
} // GameEngine
} // ForlornMelody




#endif //MUSICBOX_LIGHT_H
