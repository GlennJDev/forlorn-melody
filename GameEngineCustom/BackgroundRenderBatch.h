//
// Created by Glenn Jacob on 24/04/16.
//

#ifndef FORLORNMELODY_BACKGROUNDRENDERBATCH_H
#define FORLORNMELODY_BACKGROUNDRENDERBATCH_H

#include <glm/glm.hpp>
#include <GameEngineBase/IRenderBatch.h>
#include <GameEngineBase/GraphicsUtils.h>

using std::vector;

namespace ForlornMelody {
namespace GameEngine {

namespace Base {

class ShaderProgram;
class Camera;
class Light;
class Node;

} // Base

using Base::Vertex;
using Base::ShaderProgram;
using Base::Camera;
using Base::Light;
using Base::Node;

namespace Custom {

struct RenderBatch;

class BackgroundRenderBatch : public Base::IRenderBatch
{
public:
    BackgroundRenderBatch();
    virtual ~BackgroundRenderBatch();

    virtual Base::RenderBatchID getID() const override;
    virtual void begin() override;
    virtual void draw(Base::Node* pNode) override;
    virtual void render() override;
    virtual void end() override;
    virtual void setShaderProgram(const shared_ptr<Base::ShaderProgram>& pProgram) override;
    virtual void setCamera(const shared_ptr<Base::Camera>& pCamera) override;
    virtual void setLight(const shared_ptr<Base::Light>& pLight) override;
    virtual void setRenderArea(const glm::vec2& area) override;
    virtual void setProjectionMatrix(const glm::mat4& projectionMatrix) override;


private:
    void init();
    void createRenderBatches();
    size_t calculateVerticesCount();
    size_t calculateVerticesDrawingOrderSize();
    void sortNodes();

    void configureForBatch(ShaderProgram& shaderProgram, const RenderBatch& renderBatch) const;
    void configureForNode(ShaderProgram& shaderProgram, Node* const& pNode) const;

    shared_ptr<ShaderProgram> pShaderProgram;
    shared_ptr<Camera> pCamera;
    shared_ptr<Light> pLight;

    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint ebo = 0;
    glm::mat4 projectionMatrix;

    vector<Node*> nodes;
    vector<RenderBatch> renderBatches;
    vector<Vertex> vertices;
    vector<GLuint> drawingOrder;
};

} // Custom
} // GameEngine
} // ForlornMelody
#endif //FORLORNMELODY_BACKGROUNDRENDERBATCH_H
