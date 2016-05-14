//
// Created by Glenn Jacob on 26/12/15.
//

#include "NodeBatch.h"
#include <algorithm>
#include <set>
#include <glm/gtc/matrix_transform.hpp>
#include "Node.h"
#include "ShaderProgram.h"
#include "Camera.h"
#include "Light.h"
#include "Constants.h"
#include "Logger.h"
#include "ObjectModel.h"
#include "Chrono.h"


using std::set;

namespace ForlornMelody {
namespace GameEngine {
namespace Base {

struct RenderBatch {
    RenderBatch(GLuint offset, GLuint numVertices, GLTexture texture)
        : indicesOffset(offset), numIndices(numVertices) , texture(texture) { }

    GLuint indicesOffset;
    GLuint numIndices;
    vector<Node*> nodes;
    GLTexture texture;
};

NodeBatch::NodeBatch()
{
    this->init();
}

NodeBatch::~NodeBatch()
{
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao);
}

void NodeBatch::init()
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

void NodeBatch::begin()
{
    this->nodes.clear();
    this->renderBatches.clear();
    this->vertices.clear();
    this->drawingOrder.clear();
}

void NodeBatch::setNodeSortType(NodeSortType sortType)
{
    this->sortType = sortType;
}

void NodeBatch::draw(Node *pNode)
{
    this->nodes.push_back(pNode);
}

void NodeBatch::end()
{
    this->sortNodes();
    this->createRenderBatches();
}

void NodeBatch::render()
{
//    auto chrono = Chrono();
//    chrono.start();

    if (this->renderBatches.empty())
        return;

    // ShaderProgram
    this->pShaderProgram->use();

    if (pCamera != nullptr) {
        this->pShaderProgram->loadViewMatrix(*pCamera);
    }
    if (pLight != nullptr) {
        this->pShaderProgram->loadLightParameters(*pLight);
    }

    this->pShaderProgram->enableDepthTesting();
    this->pShaderProgram->enableCullFace();

    // Actual Rendering
    // Binding the vertex array binds all buffers in it as well
    glBindVertexArray(this->vao);
    for (auto&& renderBatch : this->renderBatches) {

        this->configureForBatch(*pShaderProgram, renderBatch);

        for (auto&& pNode : renderBatch.nodes) {

            this->configureForNode(*pShaderProgram, pNode);

            auto identity = mat4(1.0f);
            auto centerOffset = pNode->getCenterOffsetFromModelOrigin();

            const auto& nodePosition = pNode->getPosition();
            const auto& nodeScale = pNode->getScale();
            const auto& nodeRotation = pNode->getRotation();

            // Translaties naar het middelpunt van het model
            auto translateToCenter = glm::translate(identity, -(centerOffset));
            auto undoTranslateToCenter = glm::inverse(translateToCenter);

            auto scale = glm::scale(identity, nodeScale);
            auto rotX = glm::rotate(identity, nodeRotation.x, vec3(1, 0, 0));
            auto rotY = glm::rotate(identity, nodeRotation.y, vec3(0, 1, 0));
            auto rotZ = glm::rotate(identity, nodeRotation.z, vec3(0, 0, 1));

            // Berekenen waar middelpunt op de bodem is van het model
            auto boundingCube = pNode->getObjectModel()->getBoundingCube();
            auto bottomY = boundingCube.midY() - (boundingCube.height() * nodeScale.y / 2.0f);
            auto positioningPoint = vec4(boundingCube.midX(), bottomY, boundingCube.midZ(), 1);
            positioningPoint = undoTranslateToCenter * rotX * rotY * rotZ * translateToCenter * positioningPoint;

            // Translatie naar het middelpunt op de bodem om alles op dezelfde manier te kunnen positioneren
            auto translateToPositioningPoint = glm::translate(identity, -vec3(positioningPoint.x, positioningPoint.y, positioningPoint.z));
            auto undoTranslateToPositioningPoint = glm::inverse(translateToPositioningPoint);
            auto translation = glm::translate(identity, nodePosition);

            // Volledige transformatie (Let op: de transformaties die eerst komen moet achteraan!)
            auto transformation =  translation * translateToPositioningPoint * undoTranslateToCenter * rotZ * rotY * rotX * scale * translateToCenter;

            pShaderProgram->setUniformMatrix4(TRANSFORMATION_MATRIX, transformation);
            glDrawElements(GL_TRIANGLES, renderBatch.numIndices, GL_UNSIGNED_INT, (void *)(renderBatch.indicesOffset * sizeof(GLuint)));
        }
    }

    // Cleaning up
    glBindTexture(GL_TEXTURE_2D, 0);
    // Unbinding the vertex array unbinds all buffers in it as well
    glBindVertexArray(0);

    this->pShaderProgram->unuse();

//    chrono.stop();
//    Logger::LogMsg("NodeBatch::render", "duration rendering : ", chrono.duration() , "s");
}

void NodeBatch::configureForNode(ShaderProgram& shaderProgram, Node* const& pNode) const
{
    shaderProgram.loadShineVariables(pNode->getReflectivity(), pNode->getShineDamper());
}

void NodeBatch::configureForBatch(ShaderProgram& shaderProgram, const RenderBatch& renderBatch) const
{
    if (renderBatch.texture.id == 0) {
        shaderProgram.setUniformBool(USE_TEXTURE, false);
        shaderProgram.enableCullFace();
    } else {
        shaderProgram.setUniformBool(USE_TEXTURE, true);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, renderBatch.texture.id);
        shaderProgram.setUniformInt(TEXTURE_SAMPLER, 0);

        if (renderBatch.texture.hasTransparency) {
            shaderProgram.disableCullFace();
        } else {
            shaderProgram.enableCullFace();
        }
    }
}

void NodeBatch::sortNodes()
{
    switch (this->sortType) {
        case FRONT_TO_BACK:
        std::stable_sort(this->nodes.begin(), this->nodes.end(), [](Node* pNode1, Node* pNode2) {
            return pNode1->getZOrder() < pNode2->getZOrder();
        });
            break;
        case BACK_TO_FRONT:
            std::stable_sort(this->nodes.begin(), this->nodes.end(), [](Node* pNode1, Node* pNode2) {
                return pNode1->getZOrder() > pNode2->getZOrder();
            });
            break;
        case TEXTURE:
            std::stable_sort(this->nodes.begin(), this->nodes.end(), [](Node* pNode1, Node* pNode2) {
                return pNode1->getTexture().id < pNode2->getTexture().id;
            });
            break;
        default:
            break;
    }
}

void NodeBatch::createRenderBatches()
{
//    auto chrono = Chrono();
//    chrono.start();
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

//    chrono.stop();
//    Logger::LogMsg("NodeBatch::createRenderBatches", "duration creating render batches : ", chrono.duration() , "s");

}

void NodeBatch::convertToGLCoords(vec4 &pos)
{
    pos.y *= -1.0f;
    pos.x -= this->renderArea.x / 2.0f;
    pos.y += this->renderArea.y / 2.0f;
    pos.x = pos.x / (this->renderArea.x / 2.0f);
    pos.y = pos.y / (this->renderArea.y / 2.0f);
}

void NodeBatch::setRenderArea(const vec2 &renderArea)
{
    this->renderArea = renderArea;
}

void NodeBatch::incrementIndices(GLuint offset, vector<unsigned int>& nodeDrawingOrder) const
{
    for (auto&& index : nodeDrawingOrder) {
        index += offset;
    }
}

size_t NodeBatch::calculateVerticesCount()
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

size_t NodeBatch::calculateVerticesDrawingOrderSize()
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

void NodeBatch::setShaderProgram(const shared_ptr<ShaderProgram>& pProgram)
{
    this->pShaderProgram = pProgram;
}

void NodeBatch::setCamera(const shared_ptr<Camera>& pCamera)
{
    this->pCamera = pCamera;
}

void NodeBatch::setLight(const shared_ptr<Light>& pLight)
{
    this->pLight = pLight;
}

void NodeBatch::setProjectionMatrix(const glm::mat4& projectionMatrix)
{
    this->pShaderProgram->use();
    this->pShaderProgram->loadProjectionMatrix(projectionMatrix);
    this->pShaderProgram->unuse();
}

RenderBatchID NodeBatch::getID() const
{
    return NODEBATCH_RENDERER_ID;
}
} // Base
} // GameEngine
} // ForlornMelody

