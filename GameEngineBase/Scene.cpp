//
// Created by Glenn Jacob on 22/12/15.
//

#include "Scene.h"

using std::make_shared;

namespace ForlornMelody {
namespace GameEngine {
namespace Base {

Scene::Scene(const string& name)
    : Node(name, "Scene")
{
}

Scene::~Scene()
{

}

const vector<Vertex>& Scene::getVertexData()
{
    this->vertexData.clear();

    for (auto nodePair : this->childNodes) {
        auto pNode = nodePair.second;
        auto& nodeVertexData = pNode->getVertexData();
        if (vertexData.capacity() < vertexData.size() + nodeVertexData.size())
            this->vertexData.reserve(vertexData.size() + nodeVertexData.size());

        this->vertexData.insert(vertexData.end(), nodeVertexData.begin(), nodeVertexData.end());
    }

    return this->vertexData;
}

void Scene::renderNodes(IRenderBatch& nodeBatch)
{
    for (auto&& childNode : this->childNodes) {
        childNode.second->renderNodes(nodeBatch);
    }
}

const vector<unsigned int>& Scene::getVerticesDrawingOrder()
{
    this->vertexDrawingOrder.clear();

    for (auto nodePair : this->childNodes) {
        auto pNode = nodePair.second;
        auto& nodeDrawingOrder = pNode->getVerticesDrawingOrder();
        if (this->vertexDrawingOrder.capacity() < vertexDrawingOrder.size() + nodeDrawingOrder.size())
            this->vertexDrawingOrder.reserve(vertexDrawingOrder.size() + nodeDrawingOrder.size());

        this->vertexDrawingOrder.insert(this->vertexDrawingOrder.end(), nodeDrawingOrder.begin(), nodeDrawingOrder.end());
    }

    return this->vertexDrawingOrder;
}

} // Base
} // GameEngine
} // ForlornMelody

