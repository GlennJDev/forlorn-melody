//
// Created by Glenn Jacob on 05/03/16.
//

#include <GameEngineBase/Utils.h>
#include "ObjectModel.h"

namespace ForlornMelody {
namespace GameEngine {
namespace Base {

using namespace Utils::glm;

ObjectModel::ObjectModel()
{
}

ObjectModel::~ObjectModel()
{

}

vector<vec3>& ObjectModel::getVertices()
{
    return this->vertices;
}

vector<vec3>& ObjectModel::getNormals()
{
    return this->normals;
}

vector<vec2>& ObjectModel::getTextureCoords()
{
    return this->textureCoords;
}

void ObjectModel::reserve(size_t size)
{
    this->vertices.reserve(size);
    this->normals.reserve(size);
    this->textureCoords.reserve(size);
}

void ObjectModel::addVertexComposition(const ivec3& composition)
{
    this->vertexCompositions.push_back(composition);
}

void ObjectModel::addDrawingOrderIndex(const unsigned int index)
{
    this->drawingOrder.push_back(index);
}

const vector<vec3>& ObjectModel::getVertices() const
{
    return this->vertices;
}

const vector<vec3>& ObjectModel::getNormals() const
{
    return this->normals;
}

const vector<vec2>& ObjectModel::getTextureCoords() const
{
    return this->textureCoords;
}

const vector<ivec3>& ObjectModel::getVertexCompositions() const
{
    return this->vertexCompositions;
}

vector<ivec3>& ObjectModel::getVertexCompositions()
{
    return this->vertexCompositions;
}

Cube& ObjectModel::getBoundingCube()
{
    return this->boundingCube;
}

const Cube& ObjectModel::getBoundingCube() const
{
    return this->boundingCube;
}

vector<unsigned int>& ObjectModel::getDrawingOrder()
{
    return this->drawingOrder;
}

const vector<unsigned int>& ObjectModel::getDrawingOrder() const
{
    return this->drawingOrder;
}

ostream& operator<<(ostream& os, const ObjectModel& model)
{
    for (int i = 0; i < model.drawingOrder.size(); i++) {
        auto vertexIndex = model.drawingOrder[i];
        int vi = model.vertexCompositions[vertexIndex][0];
        int ti = model.vertexCompositions[vertexIndex][1];
        int ni = model.vertexCompositions[vertexIndex][2];
        os << "vertex " << i << " : ";
        os << "v: " << model.vertices[vi]
            << " vt: " << model.textureCoords[ti]
            << " vn: " << model.normals[ni] << std::endl;
    }

    return os;
}
} // Base
}
}

