//
// Created by Glenn Jacob on 05/03/16.
//

#ifndef MUSICBOX_OBJECTMODEL_H
#define MUSICBOX_OBJECTMODEL_H

#include <ostream>
#include <vector>
#include <glm/glm.hpp>
#include <GameEngineBase/GraphicsUtils.h>

using std::ostream;
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
    void addVertexComposition(const ivec3& composition);
    void addDrawingOrderIndex(const unsigned int index);
    vector<vec3>& getVertices();
    const vector<vec3>& getVertices() const;
    vector<vec3>& getNormals();
    const vector<vec3>& getNormals() const;
    vector<vec2>& getTextureCoords();
    const vector<vec2>& getTextureCoords() const;
    const vector<ivec3>& getVertexCompositions() const;
    vector<ivec3>& getVertexCompositions();
    const vector<unsigned int>& getDrawingOrder() const;
    vector<unsigned int>& getDrawingOrder();

    Cube& getBoundingCube();
    const Cube& getBoundingCube() const;

    friend ostream& operator<<(ostream& os, const ObjectModel& model);
private:
    vector<vec3> vertices;
    vector<vec3> normals;
    vector<vec2> textureCoords;
    // all unique vertex, uv, normal combination
    // ([0] -> vertex index, [1] -> texture index, [2] -> normal index)
    vector<ivec3> vertexCompositions;
    // Indices of de vertexCompositions to draw all triangles
    vector<unsigned int> drawingOrder;
    Cube boundingCube;
};

} // Base
}
}

#endif //MUSICBOX_OBJECTMODEL_H
