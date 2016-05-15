//
// Created by Glenn Jacob on 28/03/16.
//

#include <set>
#include <glm/gtc/matrix_transform.hpp>
#include "TerrainShader.h"
#include "Terrain.h"
#include "Camera.h"
#include "Light.h"
#include "Constants.h"
#include "TerrainBatch.h"
#include "GraphicsUtils.h"
#include "Logger.h"
#include "Chrono.h"

using std::set;

namespace ForlornMelody {
namespace GameEngine {
namespace Base {

struct TerrainRenderBatch
{
    TerrainRenderBatch(GLuint offset, GLuint numVertices, GLuint texture)
        : indicesOffset(offset), numIndices(numVertices), texture(texture)
    { }

    GLuint indicesOffset;
    GLuint numIndices;
    vector<Terrain*> terrains;
    GLuint texture = 0;
};

TerrainBatch::TerrainBatch()
{
    this->init();
}

TerrainBatch::~TerrainBatch()
{
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao);
}

void TerrainBatch::init()
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);
}

void TerrainBatch::begin()
{
    this->terrains.clear();
    this->renderBatches.clear();
    this->vertices.clear();
    this->drawingOrder.clear();
}

void TerrainBatch::draw(Node* pNode)
{
    this->terrains.push_back(pNode);
}

void TerrainBatch::end()
{
    this->createRenderBatches();
}

void TerrainBatch::render()
{
//    auto chrono = Chrono();
//    chrono.start();

    if (this->renderBatches.empty())
        return;

    // ShaderProgram
    this->pShaderProgram->use();

    pShaderProgram->disableAlphaBlending();
    pShaderProgram->disableDepthTesting();
    pShaderProgram->enableCullFace();

    if (pCamera != nullptr) {
        this->pShaderProgram->loadViewMatrix(*pCamera);
    }
    if (pLight != nullptr) {
        this->pShaderProgram->loadLightParameters(*pLight);
    }

    // Actual Rendering
    // Binding the vertex array binds all buffers in it as well and enables the vertex attributes
    glBindVertexArray(this->vao);

    for (auto&& renderBatch : this->renderBatches) {

        this->configureForBatch(*pShaderProgram, renderBatch);

        for (auto&& pTerrain : renderBatch.terrains) {

            this->configureForTerrain(*pShaderProgram, pTerrain);

            auto identity = mat4(1.0f);
            auto translation = glm::translate(identity, pTerrain->getPosition());

            auto transformation = identity;
            transformation *= translation;

            pShaderProgram->setUniformMatrix4(TRANSFORMATION_MATRIX, transformation);
            glDrawElements(GL_TRIANGLES,
                           renderBatch.numIndices,
                           GL_UNSIGNED_INT,
                           (void*)(renderBatch.indicesOffset * sizeof(GLuint)));
        }
    }

    // Cleaning up
    glBindTexture(GL_TEXTURE_2D, 0);
    // Unbinding the vertex array unbinds all buffers in it as well
    glBindVertexArray(0);

    this->pShaderProgram->unuse();
//    chrono.stop();
//    Logger::LogMsg("TerrainBatch::render", "duration rendering : ", chrono.duration() , "s");

//    Logger::LogMsg("TerrainBatch::render", "end");
}

void TerrainBatch::createRenderBatches()
{
//    auto chrono = Chrono();
//    chrono.start();
    if (this->terrains.empty()) {
        return;
    }

//    Logger::LogMsg("TerrainBatch::createRenderBatches", "begin");

    size_t totalVerticesCount = this->calculateVerticesCount();
    size_t totalDrawingOrderCount = this->calculateVerticesDrawingOrderSize();
    if (this->vertices.capacity() < totalVerticesCount)
        vertices.reserve(totalVerticesCount);
    if (this->drawingOrder.capacity() < totalDrawingOrderCount)
        drawingOrder.reserve(totalDrawingOrderCount);

    size_t verticesCount = 0;
    size_t drawingOrdersCount = 0;
    set<string> terrainTypes;
    GLuint offset = 0;
    int prevTextureId = -1;

//    Logger::LogMsg("TerrainBatch::createRenderBatches", "vertices count: ", totalVerticesCount, " vertex indices count: ", totalDrawingOrderCount);

    for (auto&& pTerrain : this->terrains) {
        const auto& nodeTexture = pTerrain->getTexture();

        auto category = pTerrain->getCategory();
        if (terrainTypes.count(category) == 0) {
            // Nieuw node type gevonden
            terrainTypes.insert(category);

            // Eerst de drawing order toevoegen om de juiste increment te doen
            auto& nodeDrawingOrder = pTerrain->getVerticesDrawingOrder();
            for (int i = 0; i < nodeDrawingOrder.size(); ++i) {
                drawingOrder.emplace_back(nodeDrawingOrder[i] + verticesCount);
            }
            drawingOrdersCount += nodeDrawingOrder.size();

            // Dan pas de vertices toevoegen
            auto& nodeVertices = pTerrain->getVertexData();
            for (int j = 0; j < nodeVertices.size(); ++j) {
                vertices.emplace_back(nodeVertices[j]);
            }
            verticesCount += nodeVertices.size();

            this->renderBatches.emplace_back(offset, nodeDrawingOrder.size(), nodeTexture.id);
            offset += nodeDrawingOrder.size();
        } else if (nodeTexture.id != prevTextureId) {
            // Geen nieuwe node type, maar wel een andere texture gebruikt
            auto& renderBatch = this->renderBatches.back();
            this->renderBatches.emplace_back(renderBatch.indicesOffset, renderBatch.numIndices, nodeTexture.id);
        }

        prevTextureId = nodeTexture.id;
        this->renderBatches.back().terrains.push_back(static_cast<Terrain*>(pTerrain));
    }

//    Logger::LogMsg("TerrainBatch::createRenderBatches", "after for loop");

    // Bind our VBO
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), this->vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 drawingOrder.size() * sizeof(GLuint),
                 this->drawingOrder.data(),
                 GL_STATIC_DRAW);

    // Unbind the VBO & EBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

//    chrono.stop();
//    Logger::LogMsg("TerrainBatch::createRenderBatches", "duration creating render batches : ", chrono.duration() , "s");
}

size_t TerrainBatch::calculateVerticesCount()
{
    size_t count = 0;
    set<string> terrainTypes;
    for (auto&& pTerrain : this->terrains) {
        auto category = pTerrain->getCategory();
        if (terrainTypes.count(category) == 0) {
            terrainTypes.insert(category);
            count += pTerrain->verticesCount();
        }
    }
    return count;
}

size_t TerrainBatch::calculateVerticesDrawingOrderSize()
{
    size_t count = 0;
    set<string> terrainTypes;
    for (auto&& pTerrain : this->terrains) {
        auto category = pTerrain->getCategory();
        if (terrainTypes.count(category) == 0) {
            terrainTypes.insert(category);
            count += pTerrain->drawingOrderSize();
        }
    }
    return count;
}

void TerrainBatch::setShaderProgram(const shared_ptr<ShaderProgram>& pProgram)
{
    this->pShaderProgram = pProgram;
}

void TerrainBatch::setCamera(const shared_ptr<Camera>& pCamera)
{
    this->pCamera = pCamera;
}

void TerrainBatch::setLight(const shared_ptr<Light>& pLight)
{
    this->pLight = pLight;
}

void TerrainBatch::setRenderArea(const vec2& renderArea)
{
    this->renderArea = renderArea;
}

void TerrainBatch::setProjectionMatrix(const glm::mat4& projectionMatrix)
{
    this->pShaderProgram->use();
    this->pShaderProgram->loadProjectionMatrix(projectionMatrix);
    this->pShaderProgram->unuse();
}

void TerrainBatch::configureForBatch(ShaderProgram& shaderProgram, const TerrainRenderBatch& renderBatch) const
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, renderBatch.texture);
    shaderProgram.setUniformInt(TEXTURE_SAMPLER, 0);
}

void TerrainBatch::configureForTerrain(ShaderProgram& shaderProgram, Terrain* pTerrain) const
{
    shaderProgram.loadShineVariables(pTerrain->getReflectivity(), pTerrain->getShineDamper());
}

RenderBatchID TerrainBatch::getID() const
{
    return TERRAINBATCH_RENDERER_ID;
}

}
}
}


