//
// Created by Glenn Jacob on 13/03/16.
//

#include "Constants.h"
#include "IRenderBatch.h"
#include "Terrain.h"

namespace ForlornMelody {
namespace GameEngine {
namespace Base {

const float Terrain::SIZE_X = 64;
const float Terrain::SIZE_Z = 2;
const int Terrain::VERTEX_COUNT_Z = 4;
const int Terrain::VERTEX_COUNT_X = 64;


Terrain::Terrain(const string& name, const string& category, int gridX, int gridZ, GLTexture texture)
    : Node(name, category)
{
    this->texture = texture;
    this->gridX = gridX;
    this->gridZ = gridZ;
    this->position.x = gridX * SIZE_X;
    this->position.z = gridZ * SIZE_Z;
    this->generateTerrain();
}

Terrain::~Terrain()
{
}

void Terrain::generateTerrain()
{
    int count = VERTEX_COUNT_X * VERTEX_COUNT_Z;
    this->pObjectModel = std::make_shared<ObjectModel>();
    auto& vertices = this->pObjectModel->getVertices();
    auto& normals = this->pObjectModel->getNormals();
    auto& textureCoords = this->pObjectModel->getTextureCoords();
    auto& vertexCompositions = this->pObjectModel->getVertexCompositions();
    vertices.resize((size_t)count);
    normals.resize((size_t)count);
    textureCoords.resize((size_t)count);
    vertexCompositions.resize((size_t)count);
    int vertexPointer = 0;
    for(float zi=0 ; zi<VERTEX_COUNT_Z ; zi++){
        for(float xi=0 ; xi<VERTEX_COUNT_X ; xi++){
            auto maxXi = (float)VERTEX_COUNT_X - 1;
            vertices[vertexPointer].x = xi/ maxXi * SIZE_X;
            vertices[vertexPointer].y = 0;
            auto maxZi = (float)VERTEX_COUNT_Z - 1;
            vertices[vertexPointer].z = (zi/maxZi * SIZE_Z) - SIZE_Z;
            normals[vertexPointer].x = 0;
            normals[vertexPointer].y = 1;
            normals[vertexPointer].z = 0;
            textureCoords[vertexPointer][0] = xi/ maxXi;
            textureCoords[vertexPointer][1] = zi/ maxZi;

            vertexCompositions[vertexPointer][0] = vertexPointer;
            vertexCompositions[vertexPointer][1] = vertexPointer;
            vertexCompositions[vertexPointer][2] = vertexPointer;
            
            vertexPointer++;
        }
    }

    // Size = aantal vierkanten x 6. Elk vierkant heeft 6 punten nodig
    auto& drawingOrders = pObjectModel->getDrawingOrder();
    drawingOrders.resize((unsigned long)(6*(VERTEX_COUNT_X-1)*(VERTEX_COUNT_Z-1)));
    int pointer = 0;
    for(int gz=0 ; gz<VERTEX_COUNT_Z-1 ; gz++){
        for(int gx=0 ; gx<VERTEX_COUNT_X-1 ; gx++){
            int topLeft = (gz*VERTEX_COUNT_X)+gx;
            int topRight = topLeft + 1;
            int bottomLeft = ((gz+1)*VERTEX_COUNT_X)+gx;
            int bottomRight = bottomLeft + 1;

            drawingOrders[pointer++] = topLeft;
            drawingOrders[pointer++] = bottomLeft;
            drawingOrders[pointer++] = topRight;
            drawingOrders[pointer++] = topRight;
            drawingOrders[pointer++] = bottomLeft;
            drawingOrders[pointer++] = bottomRight;
        }
    }
}

void Terrain::renderNodes(IRenderBatch& nodeBatch)
{
    if (nodeBatch.getID() == TERRAINBATCH_RENDERER_ID)
        nodeBatch.draw(this);
}
} // Custom
} // GameEngine
} // ForlornMelody

