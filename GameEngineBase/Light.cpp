//
// Created by Glenn Jacob on 12/03/16.
//

#include "Light.h"

namespace ForlornMelody {
namespace GameEngine {
namespace Base {

Light::Light()
{
}

Light::Light(const vec3& position, const vec3& color)
    : position(position), color(color)
{ }

Light::~Light()
{
}

const vec3& Light::getPosition() const
{
    return this->position;
}

void Light::setPosition(const vec3& position)
{
    this->position = position;
}

const vec3& Light::getColor() const
{
    return color;
}

void Light::setColor(const vec3& color)
{
    this->color = color;
}
} // Base
} // GameEngine
} // ForlornMelody


