//
// Created by Glenn Jacob on 24/04/16.
//

#include <GameEngineBase/Node.h>
#include <GameEngineBase/IRenderBatch.h>
#include <GameEngineBase/Constants.h>
#include <GameEngineCustom/Constants.h>
#include "ImageNode.h"

namespace ForlornMelody {
namespace GameEngine {
namespace Custom {

ImageNode::ImageNode(const string& name, const string& category, const Base::GLTexture& texture)
    :Node(name, category, texture)
{
}

ImageNode::~ImageNode()
{
}

const vector<Base::Vertex>& ImageNode::getVertexData()
{
    static const int UPPER_LEFT = 3;
    static const int UPPER_RIGHT = 2;
    static const int BOTTOM_RIGHT = 1;
    static const int BOTTOM_LEFT = 0;
    if (vertexData.size() == 0) {
        this->vertexData.resize(4);

        float width = this->size[0];
        float height = this->size[1];
        vec3 corners[4] = {
            {0.0f, 0.0f, 0.0f}, // bottom-left
            {width, 0.0f, 0.0f}, // bottom-right
            {width, height, 0.0f}, // upper-right
            {0.0, height, 0.0f}, // upper-left
        };

        vertexData[0].position = corners[UPPER_LEFT];
        vertexData[1].position = corners[BOTTOM_LEFT];
        vertexData[2].position = corners[UPPER_RIGHT];
        vertexData[3].position = corners[BOTTOM_RIGHT];
        vertexData[0].setColor(this->color.r, this->color.g, this->color.b, this->color.a);
        vertexData[1].setColor(this->color.r, this->color.g, this->color.b, this->color.a);
        vertexData[2].setColor(this->color.r, this->color.g, this->color.b, this->color.a);
        vertexData[3].setColor(this->color.r, this->color.g, this->color.b, this->color.a);
        vertexData[0].setUV(0.0f, 0.0f);
        vertexData[1].setUV(0.0f, 1.0f);
        vertexData[2].setUV(1.0f, 0.0f);
        vertexData[3].setUV(1.0f, 1.0f);
        vertexData[0].setNormal(0.0f, 0.0f, 1.0f);
        vertexData[1].setNormal(0.0f, 0.0f, 1.0f);
        vertexData[2].setNormal(0.0f, 0.0f, 1.0f);
        vertexData[3].setNormal(0.0f, 0.0f, 1.0f);
    }
    return vertexData;
}

const vector<unsigned int>& ImageNode::getVerticesDrawingOrder()
{
    if (this->vertexDrawingOrder.size() == 0)
        this->vertexDrawingOrder = {0, 1, 3, 3, 2, 0 };
    return this->vertexDrawingOrder;
}

size_t ImageNode::verticesCount() const
{
    return 4;
}

size_t ImageNode::drawingOrderSize() const
{
    return 6;
}

void ImageNode::renderNodes(Base::IRenderBatch& nodeBatch)
{
    if (nodeBatch.getID() == Custom::BACKGROUND_BATCH_RENDERER_ID) {
        nodeBatch.draw(this);
    }
}

vec3 ImageNode::getCenter() const
{
    return vec3(size[0]/2.0f, size[1]/2.0f,getZ());
}

vec3 ImageNode::getCenterOffsetFromModelOrigin() const
{
    return vec3(size[0]/2.0f, size[1]/2.0f, 0);
}
} // Custom
} // GameEngine
} // ForlornMelody

