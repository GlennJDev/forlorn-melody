//
// Created by Glenn Jacob on 22/12/15.
//

#ifndef MUSICBOX_NODE_H
#define MUSICBOX_NODE_H

#include <map>
#include <vector>
#include <string>
#include <memory>
#include <glm/glm.hpp>
#include "GraphicsUtils.h"

using std::map;
using std::vector;
using std::string;
using std::shared_ptr;
using glm::vec3;
using glm::vec4;

namespace ForlornMelody {
namespace GameEngine {
namespace Base {

class Node;
class IRenderBatch;
class ObjectModel;

using NodePtr = std::shared_ptr<Node>;

class Node
{
public:
    // Constructors for inheritance purpose
    Node(const string& name, const string& category);
    Node(const string& name, const string& category, const GLTexture& texture);

    // Main constructor
    Node(const string& name,
         const string& category,
         const shared_ptr<ObjectModel>& objectModel,
         const GLTexture& texture = GLTexture());
    virtual ~Node();

    // Getters
    virtual const string& getName() const;
    virtual const string& getCategory() const;
    virtual const vec3& getPosition() const;
    virtual vec3 getPosition();
    virtual vec3 getCenter() const;
    virtual vec3 getCenterOffsetFromModelOrigin() const;
    virtual const vec3 getSize() const;
    virtual const vec3& getScale() const;
    virtual const vec3& getRotation() const;
    virtual int getZOrder() const;
    virtual const GLTexture& getTexture() const;
    virtual const shared_ptr<ObjectModel> getObjectModel() const;
    virtual const Color& getColor() const;
    virtual float getX() const;
    virtual float getY() const;
    virtual float getZ() const;
    virtual float getXrotation() const;
    virtual float getYrotation() const;
    virtual float getZrotation() const;
    virtual float getReflectivity() const;
    virtual float getShineDamper() const;

    // Setters
    virtual void setPosition(const vec3& position);
    virtual void setScale(const vec3& scale);
    virtual void setRotation(const vec3& rotation);
    virtual void setZOrder(const int zOrder);
    virtual void setTexture(const GLTexture& texture);
    virtual void setColor(const Color& color);
    virtual void setX(const float x);
    virtual void setY(const float y);
    virtual void setZ(const float z);
    virtual void setHeight(const float height);
    virtual void setWidth(const float width);
    virtual void setDepth(const float depth);
    virtual void setXScale(const float xScale);
    virtual void setYScale(const float yScale);
    virtual void setZScale(const float zScale);
    virtual void setXRotation(const float xRot);
    virtual void setYRotation(const float yRot);
    virtual void setZRotation(const float zRot);
    virtual void setRed(const unsigned char red);
    virtual void setGreen(const unsigned char green);
    virtual void setBlue(const unsigned char blue);
    virtual void setAlpha(const unsigned char alpha);
    virtual void setReflectivity(const float reflectivity);
    virtual void setShineDamper(const float shineDamper);

    virtual const vector<Vertex>& getVertexData();
    virtual const vector<unsigned int>& getVerticesDrawingOrder();
    virtual size_t verticesCount() const;
    virtual size_t drawingOrderSize() const;

    // Nodes
    const map<string, NodePtr>& getChildNodes() const;
    void addChildNode(NodePtr pNode);
    void removeNode(const Node& node);
    void removeNodeWithName(const string& nameNode);

    // Rendering
    virtual void renderNodes(IRenderBatch& nodeBatch);

protected:
    string name; // identifier
    string category; // node type identifier
    GLTexture texture;
    shared_ptr<ObjectModel> pObjectModel;

    vec3 position {0.0f, 0.0f, 0.0f};
    vec3 size {1.0f, 1.0f, 1.0f};
    vec3 scale {1.0f, 1.0f, 1.0f};
    vec3 rotation {0.0f, 0.0f, 0.0f};
    int zOrder = 0;
    Color color {0, 0, 0, 0};
    float reflectivity = 0.f;
    // How close does the camera have to be to the reflected light.
    float shineDamper = 0.f;

    map<string, NodePtr> childNodes;

    vector<Vertex> vertexData;
    vector<unsigned int> vertexDrawingOrder;
};

} // Base
} // GameEngine
} // ForlornMelody




#endif //MUSICBOX_NODE_H
