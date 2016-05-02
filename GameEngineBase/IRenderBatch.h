//
// Created by Glenn Jacob on 13/03/16.
//

#ifndef MUSICBOX_IRENDERER_H
#define MUSICBOX_IRENDERER_H

#include <memory>
#include <glm/glm.hpp>

using std::shared_ptr;

namespace ForlornMelody {
namespace GameEngine {
namespace Base {

class ShaderProgram;
class Camera;
class Light;
class Node;

using RenderBatchID = unsigned int;

class IRenderBatch {
public:

    virtual RenderBatchID getID() const = 0;

    virtual void begin() = 0;
    virtual void draw(Node* pNode) = 0;
    virtual void render() = 0;
    virtual void end() = 0;

    virtual void setShaderProgram(const shared_ptr<ShaderProgram>& pProgram) = 0;
    virtual void setCamera(const shared_ptr<Camera>& pCamera) = 0;
    virtual void setLight(const shared_ptr<Light>& pLight) = 0;

    virtual void setRenderArea(const glm::vec2& area) = 0;
    virtual void setProjectionMatrix(const glm::mat4& projectionMatrix) = 0;
};
} // Base
}
}

#endif //MUSICBOX_IRENDERER_H
