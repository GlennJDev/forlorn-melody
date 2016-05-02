//
// Created by Glenn Jacob on 22/12/15.
//

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "MasterRenderer.h"
#include "Scene.h"
#include "ShaderProgram.h"
#include "NodeBatch.h"
#include "IRenderBatch.h"
#include "Logger.h"
#include "Chrono.h"

using std::make_shared;
using glm::mat4;
using glm::vec3;

namespace ForlornMelody {
namespace GameEngine {
namespace Base {

const vec3 MasterRenderer::skyColor = {0.0f,0.7f,0.8f};
//const vec3 MasterRenderer::skyColor = {1.f,1.f,1.f};

MasterRenderer::MasterRenderer(const vec2& renderArea)
    : renderArea(renderArea)
{
    this->initRenderer();
}

MasterRenderer::~MasterRenderer()
{

}

shared_ptr<Scene> MasterRenderer::getScene() const
{
    return this->pScene;
}
void MasterRenderer::setScene(shared_ptr<Scene> scene)
{
    this->pScene = scene;
}

void MasterRenderer::initRenderer()
{
    glClearColor(skyColor.r, skyColor.g, skyColor.b,1.0f);
    glClearDepth(1.0);
}

void MasterRenderer::render()
{
    this->prepareForRendering();
    this->renderScene();
    this->finishRendering();
}

void MasterRenderer::prepareForRendering()
{
    if (!this->pScene) {
        return;
    }

    for (auto&& renderBatchPair  : this->renderers) {
        auto pRenderBatch = renderBatchPair.second;

        pRenderBatch->begin();

        this->pScene->renderNodes(*(pRenderBatch));

        pRenderBatch->end();
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MasterRenderer::renderScene()
{
    if (!this->pScene) {
        return;
    }

    for (auto&& renderBatchPair  : this->renderers) {
        auto pRenderBatch = renderBatchPair.second;
        pRenderBatch->render();
    }
}

void MasterRenderer::finishRendering()
{
    // Tell openGL to execute all pending commands
    // https://www.opengl.org/sdk/docs/man2/xhtml/glFlush.xml
    glFlush();
}

shared_ptr<ShaderProgram> MasterRenderer::getShaderProgram() const
{
    return this->pShaderProgram;
}
void MasterRenderer::setProgram(const shared_ptr<ShaderProgram> pGLSLProgram)
{
    this->pShaderProgram = pGLSLProgram;
}

void MasterRenderer::setRenderArea(const vec2& renderArea)
{
    this->renderArea = renderArea;
    for (auto&& rendererPair  : this->renderers) {
        rendererPair.second->setRenderArea(renderArea);
    }
}

mat4 MasterRenderer::createProjectionMatrix()
{
    float FOV = 70;
    float NEAR_PLANE = .1f;
    float FAR_PLANE = 100.0f;
    float aspectRatio = this->renderArea[0] / this->renderArea[1];

    mat4 projectionMatrix = glm::perspective(glm::radians(FOV), aspectRatio, NEAR_PLANE, FAR_PLANE);

    return projectionMatrix;
}

shared_ptr<Camera> MasterRenderer::getCamera() const
{
    return this->pCamera;
}

void MasterRenderer::setCamera(shared_ptr<Camera> pCamera)
{
    this->pCamera = pCamera;
}

shared_ptr<Light> MasterRenderer::getLight() const
{
    return this->pLight;
}

void MasterRenderer::setLight(shared_ptr<Light> pLight)
{
    this->pLight = pLight;
}

void MasterRenderer::addRenderer(const string& rendererId, shared_ptr<IRenderBatch> pRenderBatch)
{
    pRenderBatch->setProjectionMatrix(this->createProjectionMatrix());
    pRenderBatch->setRenderArea(this->renderArea);
    this->renderers[rendererId] = pRenderBatch;
}

void MasterRenderer::removeRenderer(const string& rendererId)
{
    this->renderers.erase(rendererId);
}
} // Base
} // GameEngine
} // ForlornMelody

