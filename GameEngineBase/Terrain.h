//
// Created by Glenn Jacob on 13/03/16.
//

#ifndef MUSICBOX_TERRAIN_H
#define MUSICBOX_TERRAIN_H

#include <memory>
#include "Node.h"
#include "GraphicsUtils.h"
#include "ObjectModel.h"

using std::shared_ptr;

namespace ForlornMelody {
namespace GameEngine {
namespace Base {

class Terrain : public Node
{
public:
    Terrain(const string& name, const string& category, int gridX, int gridZ, GLTexture texture);
    virtual ~Terrain();

    virtual void renderNodes(IRenderBatch& nodeBatch) override;


private:

    void generateTerrain();

    float gridX;
    float gridZ;

    static const float SIZE;
    static const int VERTEX_COUNT;
};
} // Custom
} // GameEngine
} // ForlornMelody




#endif //MUSICBOX_TERRAIN_H
