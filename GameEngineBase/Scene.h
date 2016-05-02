//
// Created by Glenn Jacob on 22/12/15.
//

#ifndef MUSICBOX_SCENE_H
#define MUSICBOX_SCENE_H

#include <string>
#include "Node.h"
#include "IRenderBatch.h"

using std::string;

namespace ForlornMelody {
namespace GameEngine {
namespace Base {

class IRenderBatch;

class Scene : public Node
{
public:
    Scene(const string& name);
    virtual ~Scene();

    virtual void renderNodes(IRenderBatch& nodeBatch) override;
    virtual const vector<Vertex>& getVertexData() override;

    virtual const vector<unsigned int>& getVerticesDrawingOrder() override;
protected:

};

} // Base
} // GameEngine
} // ForlornMelody




#endif //MUSICBOX_SCENE_H
