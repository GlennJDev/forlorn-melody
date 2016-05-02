//
// Created by Glenn Jacob on 22/12/15.
//

#ifndef MUSICBOX_ISCENERENDERER_H
#define MUSICBOX_ISCENERENDERER_H

#include <memory>
#include <glm/glm.hpp>

using std::shared_ptr;
using glm::vec2;

namespace ForlornMelody {
namespace GameEngine {
namespace Base {

class Scene;
class ShaderProgram;
class Camera;
class Light;

class IRenderer
{
public:
    virtual shared_ptr<Scene> getScene() const = 0;
    virtual void setScene(const shared_ptr<Scene> scene) = 0;
    virtual shared_ptr<ShaderProgram>getShaderProgram() const = 0;
    virtual void setProgram(const shared_ptr<ShaderProgram> pShaderProgram) = 0;
    virtual shared_ptr<Camera> getCamera() const = 0;
    virtual void setCamera(shared_ptr<Camera> pCamera) = 0;
    virtual shared_ptr<Light> getLight() const = 0;
    virtual void setLight(shared_ptr<Light> pLight) = 0;
    virtual void setRenderArea(const vec2& renderArea) = 0;

    virtual void initRenderer() = 0;
    virtual void prepareForRendering() = 0;
    virtual void renderScene() = 0;
    virtual void finishRendering() = 0;
};
} // Base
} // GameEngine
} // ForlornMelody




#endif //MUSICBOX_ISCENERENDERER_H
