//
// Created by Glenn Jacob on 22/12/15.
//

#ifndef MUSICBOX_GAMEVIEW_H
#define MUSICBOX_GAMEVIEW_H

#include <memory>
#include <unordered_map>
#include <string>
#include <glm/glm.hpp>
#include "IRenderer.h"
#include "IRenderBatch.h"

using std::shared_ptr;
using std::unordered_map;
using std::string;
using glm::vec2;
using glm::vec3;
using glm::mat4;

namespace ForlornMelody {
namespace GameEngine {
namespace Base {

class Scene;
class ShaderProgram;
class ResourceCache;
class Camera;
class Light;

class MasterRenderer : public IRenderer
{
public:
    MasterRenderer(const vec2& renderArea);
    virtual ~MasterRenderer();

    virtual shared_ptr<Scene> getScene() const override;
    virtual void setScene(shared_ptr<Scene> scene) override;
    virtual shared_ptr<ShaderProgram> getShaderProgram() const override;
    virtual void setProgram(const shared_ptr<ShaderProgram> pGLSLProgram) override;
    virtual shared_ptr<Camera> getCamera() const override;
    virtual void setCamera(shared_ptr<Camera> pCamera) override;
    virtual shared_ptr<Light> getLight() const override;
    virtual void setLight(shared_ptr<Light> pLight) override;
    virtual void setRenderArea(const vec2& renderArea) override;

    virtual void initRenderer() override;
    virtual void prepareForRendering() override;
    virtual void renderScene() override;
    virtual void finishRendering() override;
    void render();

    void addRenderer(const string& rendererId, shared_ptr<IRenderBatch> pRenderBatch);
    void removeRenderer(const string& rendererId);

    mat4 createProjectionMatrix();
private:

    unordered_map<string, shared_ptr<IRenderBatch>> renderers;
    shared_ptr<Scene> pScene = nullptr;
    shared_ptr<ShaderProgram> pShaderProgram = nullptr;
    shared_ptr<Camera> pCamera = nullptr;
    shared_ptr<Light> pLight = nullptr;
    vec2 renderArea{0};

    static const vec3 skyColor;
};

} // Base
}
}





#endif //MUSICBOX_GAMEVIEW_H
