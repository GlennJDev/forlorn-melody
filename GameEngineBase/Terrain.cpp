//
// Created by Glenn Jacob on 13/03/16.
//

#include "Constants.h"
#include "IRenderBatch.h"
#include "Terrain.h"

namespace ForlornMelody {
namespace GameEngine {
namespace Base {

const float Terrain::SIZE = 100;
const int Terrain::VERTEX_COUNT = 128;

Terrain::Terrain(const string& name, const string& category, int gridX, int gridZ, GLTexture texture)
    : Node(name, category)
{
    this->texture = texture;
    this->gridX = gridX;
    this->gridZ = gridZ;
    this->position.x = gridX * SIZE;
    this->position.z = gridZ * SIZE;
    this->generateTerrain();
}

Terrain::~Terrain()
{
}

void Terrain::generateTerrain()
{
    int count = VERTEX_COUNT * VERTEX_COUNT;
    this->pObjectModel = std::make_shared<ObjectModel>();
    auto& vertices = this->pObjectModel->getVertices();
    auto& normals = this->pObjectModel->getNormals();
    auto& textureCoords = this->pObjectModel->getTextureCoords();
    auto& drawingOrder = this->pObjectModel->getVertexCompositions();
    vertices.resize((unsigned long)count);
    normals.resize((unsigned long)count);
    textureCoords.resize((unsigned long)count);
    int vertexPointer = 0;
    for(int i=0;i<VERTEX_COUNT;i++){
        for(int j=0;j<VERTEX_COUNT;j++){
            vertices[vertexPointer].x = (float)j/((float)VERTEX_COUNT - 1) * SIZE;
            vertices[vertexPointer].y = 0;
            vertices[vertexPointer].z = (float)i/((float)VERTEX_COUNT - 1) * SIZE;
            normals[vertexPointer].x = 0;
            normals[vertexPointer].y = 1;
            normals[vertexPointer].z = 0;
            textureCoords[vertexPointer][0] = (float)j/((float)VERTEX_COUNT - 1);
            textureCoords[vertexPointer][1] = (float)i/((float)VERTEX_COUNT - 1);
            vertexPointer++;
        }
    }

    // Size = aantal vierkanten x 6. Elk vierkant heeft 6 punten nodig
    drawingOrder.resize((unsigned long)(6*(VERTEX_COUNT-1)*(VERTEX_COUNT-1)));
    int pointer = 0;
    for(int gz=0;gz<VERTEX_COUNT-1;gz++){
        for(int gx=0;gx<VERTEX_COUNT-1;gx++){
            int topLeft = (gz*VERTEX_COUNT)+gx;
            int topRight = topLeft + 1;
            int bottomLeft = ((gz+1)*VERTEX_COUNT)+gx;
            int bottomRight = bottomLeft + 1;
            drawingOrder[pointer][0] = topLeft;
            drawingOrder[pointer][1] = topLeft;
            drawingOrder[pointer++][2] = topLeft;
            drawingOrder[pointer][0] = bottomLeft;
            drawingOrder[pointer][1] = bottomLeft;
            drawingOrder[pointer++][2] = bottomLeft;
            drawingOrder[pointer][0] = topRight;
            drawingOrder[pointer][1] = topRight;
            drawingOrder[pointer++][2] = topRight;
            drawingOrder[pointer][0] = topRight;
            drawingOrder[pointer][1] = topRight;
            drawingOrder[pointer++][2] = topRight;
            drawingOrder[pointer][0] = bottomLeft;
            drawingOrder[pointer][1] = bottomLeft;
            drawingOrder[pointer++][2] = bottomLeft;
            drawingOrder[pointer][0] = bottomRight;
            drawingOrder[pointer][1] = bottomRight;
            drawingOrder[pointer++][2] = bottomRight;
        }
    }
}

void Terrain::renderNodes(IRenderBatch& nodeBatch)
{
    if (nodeBatch.getID() == NODEBATCH_RENDERER_ID)
        nodeBatch.draw(this);
}
} // Custom
} // GameEngine
} // ForlornMelody

