//
// Created by Glenn Jacob on 05/03/16.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <cstdlib>
#include "Utils.h"
#include "ObjectModel.h"
#include "ObjectModelLoader.h"
#include "Logger.h"


using std::ifstream;
using std::stringstream;
using std::cerr;
using std::endl;

namespace ForlornMelody {
namespace GameEngine {
namespace Base {


shared_ptr<ObjectModel> ObjectModelLoader::loadObjModel(const string &filePath)
{
    shared_ptr<ObjectModel> pModel = std::make_shared<ObjectModel>();

    try {
        auto fileReader = std::ifstream(filePath);

        if (!fileReader.is_open()) {
            Logger::logError("ObjectModelLoader::loadObjModel", "filepath (", filePath, ") does not exist");
            exit(EXIT_FAILURE);
        }

        float minX = 9999, maxX = -9999;
        float minY = 9999, maxY = -9999;
        float minZ = 9999, maxZ = -9999;

        string line;
        vector<string> pieces;
        bool faceFound = false;
        while(fileReader.good() && !faceFound) {
            getline(fileReader, line);
            pieces.clear();
            Utils::String::split(line, ' ', pieces);
            if (pieces.size() > 0) {
                if (pieces[0] == "v") {
                    vec3 vertex {atof(pieces[1].c_str()), atof(pieces[2].c_str()), atof(pieces[3].c_str())};
                    pModel->getVertices().push_back(vertex);

                    if (vertex.x < minX)
                        minX = vertex.x;
                    if (vertex.x > maxX)
                        maxX = vertex.x;
                    if (vertex.y < minY)
                        minY = vertex.y;
                    if (vertex.y > maxY)
                        maxY = vertex.y;
                    if (vertex.z < minZ)
                        minZ = vertex.z;
                    if (vertex.z > maxZ)
                        maxZ = vertex.z;
                } else if (pieces[0] == "vt") {
                    // blend heeft (u,v) = (0,0) in de linker onderhoek ipv linker bovenhoek
                    vec2 textureCoord {atof(pieces[1].c_str()), 1.0f - atof(pieces[2].c_str())};
                    pModel->getTextureCoords().push_back(textureCoord);
                } else if (pieces[0] == "vn") {
                    vec3 normal {atof(pieces[1].c_str()), atof(pieces[2].c_str()), atof(pieces[3].c_str())};
                    pModel->getNormals().push_back(normal);
                } else if (pieces[0] == "f") {
                    faceFound = true;
                }
            }
        }

        auto& boundingCube = pModel->getBoundingCube();
        boundingCube.farUpperLeft = {minX, maxY, minZ};
        boundingCube.nearUpperLeft = {minX, maxY, maxZ};
        boundingCube.farUpperRight = {maxX, maxY, minZ};
        boundingCube.nearUpperRight = {maxX, maxY, maxZ};
        boundingCube.farBottomLeft = {minX, minY, minZ};
        boundingCube.nearBottomLeft = {minX, minY, maxZ};
        boundingCube.farBottomRight = {maxX, minY, minZ};
        boundingCube.nearBottomRight = {maxX, minY, maxZ};

        pieces.clear();
        Utils::String::split(line, ' ', pieces);
        processVertices(pModel, pieces);
        while (fileReader.good()) {
            getline(fileReader, line);
            pieces.clear();
            Utils::String::split(line, ' ', pieces);
            if (pieces.size() == 4 && pieces[0] == "f") {
                processVertices(pModel, pieces);
            }
        }
    } catch (...) {
        cerr << "Something went wrong with loading the obj file " << filePath << endl;
    }
    return pModel;
}

void ObjectModelLoader::processVertices(shared_ptr<ObjectModel> pModel,
                                        const vector<string>& pieces)
{
    for (int i = 1; i < pieces.size(); i++) {
        auto indicesStr = pieces[i];
        vector<string> indices;
        indices.reserve(3);
        Utils::String::split(indicesStr, '/', indices);

        int vi = atoi(indices[0].c_str()) -1; // vertex index
        int ti = atoi(indices[1].c_str()) -1; // texture coordinates index
        int ni = atoi(indices[2].c_str()) -1; // normal index
        pModel->addDrawingIndices({vi, ti, ni});
    }
}

} // Base
} // GameEngine
} // ForlornMelody

