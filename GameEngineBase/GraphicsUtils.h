//
// Created by Glenn Jacob on 23/12/15.
//

#ifndef MUSICBOX_GRAPHICSUTILS_H
#define MUSICBOX_GRAPHICSUTILS_H

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

using std::vector;
using glm::vec2;
using glm::vec3;
using glm::vec4;

namespace ForlornMelody {
namespace GameEngine {
namespace Base {

struct Color
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
};

struct GLTexture
{
    GLuint id = 0;
    float width;
    float height;
    bool hasTransparency = false;
};

struct Vertex
{
    vec3 position;
    Color color;
    vec2 uv;
    vec3 normal;

    Vertex() {}
    Vertex(const vec3& pos, const Color& color, const vec2& uv, const vec3& normal)
        : position(pos), color(color), uv(uv), normal(normal) {}

    void setPosition(float x, float y, float z)
    {
        this->position = vec3(x, y, z);
    }

    void setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
    {
        this->color = {r, g, b, a};
    }

    void setUV(float u, float v)
    {
        uv.x = u;
        uv.y = v;
    }

    void setNormal(float x, float y, float z)
    {
        normal = {x, y, z};
    }
};

struct Cube {
    vec3 farUpperLeft;
    vec3 nearUpperLeft;
    vec3 farUpperRight;
    vec3 nearUpperRight;
    vec3 farBottomLeft;
    vec3 nearBottomLeft;
    vec3 farBottomRight;
    vec3 nearBottomRight;

    vec3 center() {
        auto x = midX();
        auto y = midY();
        auto z = midZ();
        return vec3(x, y, z);
    }

    void scale(const vec3& scale) {
        auto x = midX();
        auto y = midY();
        auto z = midZ();

        vector<vec3*> points = {&farUpperLeft, &farBottomLeft, &nearUpperLeft, &nearBottomLeft,
                                &farUpperRight, &farBottomRight, &nearUpperRight, &nearBottomRight};
        for (int i=0 ; i<8 ; i++) {
            auto& point = *points[i];
            point.x = (point.x - x)/2.0f + x;
            point.y = (point.y - y)/2.0f + y;
            point.z = (point.z - z)/2.0f + z;
        }
    }

    float midX() {
        return (farUpperRight.x - farUpperLeft.x) / 2.0f + farUpperLeft.x;
    }

    float midY() {
        return (farUpperLeft.y - farBottomLeft.y) / 2.0f + farBottomLeft.y;
    }

    float midZ() {
        return (nearUpperLeft.z - farUpperLeft.z) / 2.0f + farUpperLeft.z;
    }

    float width() {
        return farUpperRight.x - farUpperLeft.x;
    }
    float height() {
        return farUpperLeft.y - farBottomRight.y;
    }
    float depth() {
        return nearUpperLeft.z - farUpperLeft.z;
    }
};
}
}
}

#endif //MUSICBOX_GRAPHICSUTILS_H
