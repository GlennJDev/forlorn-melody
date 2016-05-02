//
// Created by Glenn Jacob on 24/04/16.
//

#ifndef FORLORNMELODY_IMAGENODE_H
#define FORLORNMELODY_IMAGENODE_H

#include <GameEngineBase/Node.h>

namespace ForlornMelody {
namespace GameEngine {
namespace Custom {

class ImageNode : public Base::Node
{
public:
    ImageNode(const string& name, const string& category, const Base::GLTexture& texture);
    virtual ~ImageNode();

    virtual const vector<Base::Vertex>& getVertexData() override;
    virtual const vector<unsigned int>& getVerticesDrawingOrder() override;
    virtual size_t verticesCount() const override;
    virtual size_t drawingOrderSize() const override;
    virtual void renderNodes(Base::IRenderBatch& nodeBatch) override;

    virtual vec3 getCenter() const override;

    virtual vec3 getCenterOffsetFromModelOrigin() const override;


private:
};

} // Custom
} // GameEngine
} // ForlornMelody




#endif //FORLORNMELODY_IMAGENODE_H
