//
// Created by Glenn Jacob on 05/03/16.
//

#ifndef MUSICBOX_OBJECTMODEL_H
#define MUSICBOX_OBJECTMODEL_H

#include <vector>
#include <glm/glm.hpp>
#include <GameEngineBase/GraphicsUtils.h>

using std::vector;
using glm::vec3;
using glm::vec2;
using glm::ivec3;

namespace ForlornMelody {
namespace GameEngine {
namespace Base {

class ObjectModel
{
public:
    ObjectModel();
    virtual ~ObjectModel();

    void reserve(size_t size);
    void addDrawingIndices(const ivec3& indices);
    vector<vec3>& getVertices();
    const vector<vec3>& getVertices() const;
    vector<vec3>& getNormals();
    const vector<vec3>& getNormals() const;
    vector<vec2>& getTextureCoords();
    const vector<vec2>& getTextureCoords() const;
    const vector<ivec3>& getDrawingOrder() const;
    vector<ivec3>& getDrawingOrder();

    Cube& getBoundingCube();
    const Cube& getBoundingCube() const;
private:
    vector<vec3> vertices;
    vector<vec3> normals;
    vector<vec2> textureCoords;
    // [0] -> vertex index, [1] -> texture index, [2] -> normal index
    vector<ivec3> drawingOrder;
    Cube boundingCube;
};

} // Base
}
}

#endif //MUSICBOX_OBJECTMODEL_H
