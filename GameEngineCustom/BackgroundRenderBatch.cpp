//
// Created by Glenn Jacob on 24/04/16.
//

#include <memory>
#include <vector>
#include <set>
#include <algorithm>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <GameEngineBase/Node.h>
#include <GameEngineBase/ShaderProgram.h>
#include <GameEngineBase/Camera.h>
#include <GameEngineBase/Light.h>
#include <GameEngineBase/Constants.h>
#include <GameEngineCustom/Constants.h>
#include "BackgroundRenderBatch.h"

using std::set;

namespace ForlornMelody {
namespace GameEngine {

using Base::GLTexture;

namespace Custom {

struct RenderBatch {
    RenderBatch(GLuint offset, GLuint numVertices, GLTexture texture)
        : indicesOffset(offset), numIndices(numVertices) , texture(texture) { }

    GLuint indicesOffset;
    GLuint numIndices;
    vector<Node*> nodes;
    GLTexture texture;
};

BackgroundRenderBatch::BackgroundRenderBatch()
{
    init();
}

BackgroundRenderBatch::~BackgroundRenderBatch()
{
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao);
}

void BackgroundRenderBatch::init()
{
    if (this->vao == 0)
        glGenVertexArrays(1, &vao);

    glBindVertexArray(this->vao);

    if (this->ebo == 0)
        glGenBuffers(1, &ebo);

    if (this->vbo == 0)
        glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);

    /* void glVertexAttribPointer
     * GLuint index: positie van de vertex input variabele
     * GLint size: het aantal waarden dat je zal meegeven
     * GLenum type: het type van de waarde dat je zal meegeven
     * GLboolean normalized: moet de waarde geconverteerd worden zodat het tussen -1 en 1 ligt
     * GLsizei stride: grootte van de volledige datastructuur
     * const GLvoid * pointer: positie in de datastructuur van de mee te geven waarden
     * Deze functie geeft aan openGL mee welke en hoeveel gegevens zullen worden meegegeven.
     * */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void *)offsetof(Vertex, color));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, uv));
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

//    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Base::RenderBatchID BackgroundRenderBatch::getID() const
{
    return BACKGROUND_BATCH_RENDERER_ID;
}

/*************************************************************/
/*                  Prepare for rendering                    */
/*************************************************************/

void BackgroundRenderBatch::begin()
{
    this->nodes.clear();
    this->renderBatches.clear();
    this->vertices.clear();
    this->drawingOrder.clear();
}

void BackgroundRenderBatch::end()
{
    this->sortNodes();
    this->createRenderBatches();
}

void BackgroundRenderBatch::draw(Base::Node* pNode)
{
    this->nodes.push_back(pNode);
}

void BackgroundRenderBatch::sortNodes()
{
    std::stable_sort(this->nodes.begin(), this->nodes.end(), [](Node* pNode1, Node* pNode2) {
        return (pNode1->getZOrder() == pNode2->getZOrder() && pNode1->getTexture().id < pNode2->getTexture().id) ||
            pNode1->getZOrder() > pNode2->getZOrder();
    });
}

void BackgroundRenderBatch::createRenderBatches()
{
    if (this->nodes.empty()) {
        return;
    }

    size_t totalVerticesCount = this->calculateVerticesCount();
    size_t totalDrawingOrderCount = this->calculateVerticesDrawingOrderSize();
    if (this->vertices.capacity() < totalVerticesCount)
        vertices.reserve(totalVerticesCount);
    if (this->drawingOrder.capacity() < totalDrawingOrderCount)
        drawingOrder.reserve(totalDrawingOrderCount);

    size_t verticesCount = 0;
    size_t drawingOrdersCount = 0;
    set<string> nodeTypes;
    GLuint offset = 0;
    int prevTextureId = -1;

//    Logger::LogMsg("NodeBatch::createRenderBatches", "vertices count: ", totalVerticesCount, " vertex indices count: ", totalDrawingOrderCount);

    for (int ni = 0; ni < this->nodes.size(); ni++) {
        auto pNode = this->nodes[ni];
        const auto& nodeTexture = pNode->getTexture();

        auto category = pNode->getCategory();
        if (nodeTypes.count(category) == 0) {
            nodeTypes.insert(category);

            auto& nodeDrawingOrder = pNode->getVerticesDrawingOrder();
            for (int i = 0; i < nodeDrawingOrder.size(); ++i) {
                drawingOrder.emplace_back(nodeDrawingOrder[i] + verticesCount);
            }
            drawingOrdersCount += nodeDrawingOrder.size();

            // Dan pas de vertices toevoegen
            auto& nodeVertices = pNode->getVertexData();
            for (int j = 0; j < nodeVertices.size(); ++j) {
                vertices.emplace_back(nodeVertices[j]);
            }
            verticesCount += nodeVertices.size();

            this->renderBatches.emplace_back(offset, nodeDrawingOrder.size(), nodeTexture);
            offset += nodeDrawingOrder.size();
        } else if (nodeTexture.id != prevTextureId) {
            // Geen nieuwe node type, maar wel een andere texture gebruikt
            auto& renderBatch = this->renderBatches.back();
            this->renderBatches.emplace_back(renderBatch.indicesOffset, renderBatch.numIndices, nodeTexture);
        }

        prevTextureId = nodeTexture.id;
        this->renderBatches.back().nodes.push_back(pNode);
    }

    // Bind our VBO
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, totalVerticesCount * sizeof(Vertex), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, totalDrawingOrderCount * sizeof(GLuint), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, drawingOrder.size() * sizeof(GLuint), drawingOrder.data());

    // Unbind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

size_t BackgroundRenderBatch::calculateVerticesCount()
{
    size_t count = 0;
    set<string> terrainTypes;
    for (auto&& pTerrain : this->nodes) {
        auto category = pTerrain->getCategory();
        if (terrainTypes.count(category) == 0) {
            terrainTypes.insert(category);
            count += pTerrain->verticesCount();
        }
    }
    return count;
}

size_t BackgroundRenderBatch::calculateVerticesDrawingOrderSize()
{
    size_t count = 0;
    set<string> terrainTypes;
    for (auto&& pNode : this->nodes) {
        auto category = pNode->getCategory();
        if (terrainTypes.count(category) == 0) {
            terrainTypes.insert(category);
            count += pNode->drawingOrderSize();
        }
    }
    return count;
}

/*************************************************************/
/*                           Render                          */
/*************************************************************/

void BackgroundRenderBatch::render()
{
    if (this->renderBatches.empty())
        return;

    // ShaderProgram
    this->pShaderProgram->use();

    if (pCamera != nullptr) {
        this->pShaderProgram->loadViewMatrix(*pCamera);
    }
    this->pShaderProgram->enableDepthTesting();

    // Actual Rendering
    // Binding the vertex array binds all buffers in it as well
    glBindVertexArray(this->vao);
    for (auto&& renderBatch : this->renderBatches) {

        this->configureForBatch(*pShaderProgram, renderBatch);

        for (auto&& pNode : renderBatch.nodes) {

            this->configureForNode(*pShaderProgram, pNode);

            const auto& nodePosition = pNode->getPosition();
            const auto& nodeScale = pNode->getScale();

            auto identity = mat4(1.0f);

            auto scale = glm::scale(identity, nodeScale);
            auto translation = glm::translate(identity, pNode->getPosition());

            auto transformation = translation * scale * identity;

            pShaderProgram->setUniformMatrix4(Base::TRANSFORMATION_MATRIX, transformation);
            glDrawElements(GL_TRIANGLES, renderBatch.numIndices, GL_UNSIGNED_INT, (void *)(renderBatch.indicesOffset * sizeof(GLuint)));
        }
    }

    // Cleaning up
    glBindTexture(GL_TEXTURE_2D, 0);
    // Unbinding the vertex array unbinds all buffers in it as well
    glBindVertexArray(0);

    this->pShaderProgram->unuse();
}

void BackgroundRenderBatch::configureForBatch(ShaderProgram& shaderProgram, const RenderBatch& renderBatch) const
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, renderBatch.texture.id);
    shaderProgram.setUniformInt(Base::TEXTURE_SAMPLER, 0);
}

void BackgroundRenderBatch::configureForNode(ShaderProgram& shaderProgram, Node* const& pNode) const
{
}

void BackgroundRenderBatch::setShaderProgram(const shared_ptr<Base::ShaderProgram>& pProgram)
{
    this->pShaderProgram = pProgram;
}

void BackgroundRenderBatch::setCamera(const shared_ptr<Base::Camera>& pCamera)
{
    this->pCamera = pCamera;
}

void BackgroundRenderBatch::setLight(const shared_ptr<Base::Light>& pLight)
{
    this->pLight = pLight;
}

void BackgroundRenderBatch::setRenderArea(const glm::vec2& area)
{
}

void BackgroundRenderBatch::setProjectionMatrix(const glm::mat4& projectionMatrix)
{
    this->projectionMatrix = projectionMatrix;
    this->pShaderProgram->use();
    this->pShaderProgram->loadProjectionMatrix(projectionMatrix);
    this->pShaderProgram->unuse();
}

} // Custom
} // GameEngine
} // ForlornMelody

