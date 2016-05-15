//
// Created by Glenn Jacob on 28/03/16.
//

#ifndef MUSICBOX_TERRAINBATCH_H
#define MUSICBOX_TERRAINBATCH_H

#include <memory>
#include <GL/glew.h>
#include "GraphicsUtils.h"
#include "IRenderBatch.h"


namespace ForlornMelody {
namespace GameEngine {
namespace Base {

struct TerrainRenderBatch;
class Node;
class Terrain;
class ShaderProgram;

class TerrainBatch : public IRenderBatch
{
public:
    TerrainBatch();
    virtual ~TerrainBatch();

    virtual RenderBatchID getID() const override;

    virtual void begin() override;
    virtual void draw(Node* pNode) override;
    virtual void render() override;
    virtual void end() override;

    virtual void setShaderProgram(const shared_ptr<ShaderProgram>& pProgram) override;
    virtual void setCamera(const shared_ptr<Camera>& pCamera) override;
    virtual void setLight(const shared_ptr<Light>& pLight) override;

    virtual void setRenderArea(const vec2& renderArea) override;
    virtual void setProjectionMatrix(const glm::mat4& projectionMatrix) override;

private:
    void init();
    void createRenderBatches();
    size_t calculateVerticesCount();
    size_t calculateVerticesDrawingOrderSize();

    void configureForBatch(ShaderProgram& shaderProgram, const TerrainRenderBatch& renderBatch) const;
    void configureForTerrain(ShaderProgram& shaderProgram, Terrain* pTerrain) const;

    vector<Node*> terrains;
    vector<TerrainRenderBatch> renderBatches;

    shared_ptr<ShaderProgram> pShaderProgram;
    shared_ptr<Camera> pCamera;
    shared_ptr<Light> pLight;

    vector<Vertex> vertices;
    vector<GLuint> drawingOrder;

    GLuint  vao = 0;
    GLuint vbo = 0;
    GLuint ebo = 0;

    vec2 renderArea {0};
};

} // Base
}
}


#endif //MUSICBOX_TERRAINBATCH_H
