//
// Created by Glenn Jacob on 26/12/15.
//

#ifndef MUSICBOX_NODEBATCH_H
#define MUSICBOX_NODEBATCH_H

#include <memory>
#include <vector>
#include <GL/glew.h>
#include "GraphicsUtils.h"
#include "IRenderBatch.h"

using std::shared_ptr;
using std::vector;

namespace ForlornMelody {
namespace GameEngine {
namespace Base {

struct RenderBatch;
class Node;
class ShaderProgram;

enum NodeSortType {
    NO_SORT,
    FRONT_TO_BACK,
    BACK_TO_FRONT,
    TEXTURE
};

class NodeBatch : public IRenderBatch
{
public:
    NodeBatch();
    virtual ~NodeBatch();

    void setNodeSortType(NodeSortType sortType);

    virtual RenderBatchID getID() const override;

    virtual void begin() override;
    virtual void draw(Node *pNode) override;
    virtual void render() override;
    virtual void end() override;

    virtual void setShaderProgram(const shared_ptr<ShaderProgram>& pProgram) override;
    virtual void setCamera(const shared_ptr<Camera>& pCamera) override;
    virtual void setLight(const shared_ptr<Light>& pLight) override;

    virtual void setRenderArea(const vec2& renderArea) override;
    virtual void setProjectionMatrix(const glm::mat4& projectionMatrix) override;

private:
    void init();
    void sortNodes();
    void createRenderBatches();
    void convertToGLCoords(vec4 &pos);
    void incrementIndices(GLuint offset, vector<unsigned int>& nodeDrawingOrder) const;
    size_t calculateVerticesCount();
    size_t calculateVerticesDrawingOrderSize();

    void configureForBatch(ShaderProgram& shaderProgram, const RenderBatch& renderBatch) const;
    void configureForNode(ShaderProgram& shaderProgram, Node* const& pNode) const;

    shared_ptr<ShaderProgram> pShaderProgram;
    shared_ptr<Camera> pCamera;
    shared_ptr<Light> pLight;

    NodeSortType sortType = NO_SORT;
    vector<Node*> nodes;
    vector<RenderBatch> renderBatches;

    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint ebo = 0;

    vec2 renderArea {0};

    vector<Vertex> vertices;
    vector<GLuint> drawingOrder;
};

} // Base
} // GameEngine
} // ForlornMelody


#endif //MUSICBOX_NODEBATCH_H
