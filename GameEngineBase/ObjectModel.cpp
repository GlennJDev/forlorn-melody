//
// Created by Glenn Jacob on 05/03/16.
//

#include "ObjectModel.h"

namespace ForlornMelody {
namespace GameEngine {
namespace Base {


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

void ObjectModel::addDrawingIndices(const ivec3& indices)
{
    this->drawingOrder.push_back(indices);
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

const vector<ivec3>& ObjectModel::getDrawingOrder() const
{
    return this->drawingOrder;
}

vector<ivec3>& ObjectModel::getDrawingOrder()
{
    return this->drawingOrder;
}

Cube& ObjectModel::getBoundingCube()
{
    return this->boundingCube;
}

const Cube& ObjectModel::getBoundingCube() const
{
    return this->boundingCube;
}
} // Base
}
}

