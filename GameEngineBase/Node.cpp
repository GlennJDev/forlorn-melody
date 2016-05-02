//
// Created by Glenn Jacob on 22/12/15.
//

#include <unordered_set>
#include "ObjectModel.h"
#include "NodeBatch.h"
#include "Constants.h"
#include "Node.h"

using std::unordered_set;

namespace ForlornMelody {
namespace GameEngine {
namespace Base {

/************************************************************/
/*               Constructor & Desctructor                  */
/************************************************************/

Node::Node(const string &name, const string &category)
    : name(name), category(category)
{}

Node::Node(const string &name, const string &category, const GLTexture &texture)
    : name(name), category(category), texture(texture)
{}

Node::Node(const string& name,
           const string& category,
           const shared_ptr<ObjectModel>& objectModel,
           const GLTexture& texture)
    : name(name), category(category), texture(texture), pObjectModel(objectModel)
{
    auto& boundingCube = objectModel->getBoundingCube();
    size = vec3(boundingCube.width(),boundingCube.height(), boundingCube.depth());
}

Node::~Node()
{}

/************************************************************/
/*                         Getters                          */
/************************************************************/
const string &Node::getName() const
{
    return this->name;
}

const string &Node::getCategory() const
{
    return this->category;
}

const vec3 &Node::getPosition() const
{
    return this->position;
}

vec3 Node::getPosition()
{
    return this->position;
}

vec3 Node::getCenter() const
{
    auto& boundingCube =  pObjectModel->getBoundingCube();
    return boundingCube.center() + this->position;
}

const vec3 Node::getSize() const
{
    return size * scale;
}

const vec3 &Node::getScale() const
{
    return this->scale;
}

const vec3 &Node::getRotation() const
{
    return this->rotation;
}

int Node::getZOrder() const
{
    return this->zOrder;
}

const GLTexture &Node::getTexture() const
{
    return this->texture;
}

const shared_ptr<ObjectModel> Node::getObjectModel() const
{
    return this->pObjectModel;
}

const Color &Node::getColor() const
{
    return this->color;
}

float Node::getX() const
{
    return this->position.x;
}

float Node::getY() const
{
    return this->position.y;
}

float Node::getZ() const
{
    return this->position.z;
}

float Node::getXrotation() const
{
    return this->rotation.x;
}

float Node::getYrotation() const
{
    return this->rotation.y;
}

float Node::getZrotation() const
{
    return this->rotation.z;
}

float Node::getReflectivity() const
{
    return this->reflectivity;
}

float Node::getShineDamper() const
{
    return this->shineDamper;
}

/************************************************************/
/*                         Setters                          */
/************************************************************/

void Node::setPosition(const vec3 &position)
{
    this->position = position;
}

void Node::setScale(const vec3 &scale)
{
    this->scale = scale;
    //this->pObjectModel->getBoundingCube().scale(scale);
}

void Node::setRotation(const vec3 &rotation)
{
    this->rotation = rotation;
}

void Node::setZOrder(const int zOrder)
{
    this->zOrder = zOrder;
}

void Node::setTexture(const GLTexture &texture)
{
    this->texture = texture;
}

void Node::setColor(const Color &color)
{
    this->color = color;
}

void Node::setX(const float x)
{
    this->position.x = x;
}

void Node::setY(const float y)
{
    this->position.y = y;
}

void Node::setZ(const float z)
{
    this->position.z = z;
}

void Node::setWidth(const float width)
{
    this->size[0] = width;
}

void Node::setHeight(const float height)
{
    this->size[1] = height;
}

void Node::setDepth(const float depth)
{
    this->size[2] = depth;
}

void Node::setXScale(const float xScale)
{
    this->setScale({xScale, scale.y, scale.z});
}

void Node::setYScale(const float yScale)
{
    this->setScale({scale.x, yScale, scale.z});
}

void Node::setZScale(const float zScale)
{
    this->setScale({scale.x, scale.y, zScale});
}

void Node::setXRotation(const float xRot)
{
    this->rotation.x = xRot;
}

void Node::setYRotation(const float yRot)
{
    this->rotation.y = yRot;
}

void Node::setZRotation(const float zRot)
{
    this->rotation.z = zRot;
}

void Node::setRed(const unsigned char red)
{
    this->color.r = red;
}
void Node::setGreen(const unsigned char green)
{
    this->color.g = green;
}
void Node::setBlue(const unsigned char blue)
{
    this->color.b = blue;
}
void Node::setAlpha(const unsigned char alpha)
{
    this->color.a = alpha;
}

void Node::setReflectivity(const float reflectivity)
{
    this->reflectivity = reflectivity;
}

void Node::setShineDamper(const float shineDamper)
{
    this->shineDamper = shineDamper;
}

void Node::addChildNode(NodePtr pNode)
{
    auto pair = std::make_pair(pNode->getName(), pNode);
    this->childNodes.insert(pair);
}

void Node::removeNode(const Node &node)
{
    this->removeNodeWithName(node.getName());
}

void Node::removeNodeWithName(const string &nameNode)
{
    this->childNodes.erase(nameNode);
}

const map<string, NodePtr> &Node::getChildNodes() const
{
    return this->childNodes;
}

const vector<Vertex>& Node::getVertexData()
{
    if (this->vertexData.size() == 0 && this->pObjectModel != nullptr) {
        auto verticesCount = this->pObjectModel->getVertices().size();
        this->vertexData.resize(verticesCount);
        int i=0;
        unordered_set<int> verticesAdded;
        verticesAdded.reserve(verticesCount);
        auto& drawingOrder = this->pObjectModel->getDrawingOrder();
        while (i<drawingOrder.size() && verticesAdded.size() < verticesCount) {
            auto& indices = drawingOrder[i];
            int vi = indices[0];
            if (verticesAdded.count(vi) == 0) {
                auto& vertex = this->pObjectModel->getVertices()[vi];
                auto& textureCoords = this->pObjectModel->getTextureCoords()[indices[1]];
                auto& normal = this->pObjectModel->getNormals()[indices[2]];

                Vertex v {vertex, this->color, textureCoords, normal};
                this->vertexData[vi] = v;
            }
            i++;
        }
    }

    return this->vertexData;
}

const vector<unsigned int>& Node::getVerticesDrawingOrder()
{
    if (this->pObjectModel != nullptr) {
        auto& drawingOrder = this->pObjectModel->getDrawingOrder();
        if (this->vertexDrawingOrder.size() == 0) {
            this->vertexDrawingOrder.reserve(pObjectModel->getDrawingOrder().size());
            for (size_t i = 0; i < drawingOrder.size(); i++) {
                vertexDrawingOrder.emplace_back(drawingOrder[i][0]);
            }
        }
    }

    return this->vertexDrawingOrder;
}

void Node::renderNodes(IRenderBatch& nodeBatch)
{
    if (nodeBatch.getID() == NODEBATCH_RENDERER_ID)
        nodeBatch.draw(this);
}

size_t Node::verticesCount() const
{
    if (this->pObjectModel != nullptr) {
        return pObjectModel->getVertices().size();
    }
    return 0;
}

size_t Node::drawingOrderSize() const
{
    if (this->pObjectModel != nullptr) {
        return this->pObjectModel->getDrawingOrder().size();
    }
    return 0;
}

vec3 Node::getCenterOffsetFromModelOrigin() const
{
    return pObjectModel->getBoundingCube().center();
}
} // Base
} // GameEngine
} // ForlornMelody

