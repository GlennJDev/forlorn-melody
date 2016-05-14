//
// Created by Glenn Jacob on 05/03/16.
//

#ifndef MUSICBOX_OBJLOADER_H
#define MUSICBOX_OBJLOADER_H

#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include <GameEngineBase/Utils.h>
#include "GraphicsUtils.h"

using std::vector;
using std::string;
using std::shared_ptr;
using std::unordered_map;

namespace ForlornMelody {
namespace GameEngine {
namespace Base {

class ObjectModel;

class ObjectModelLoader
{
public:
    static shared_ptr<ObjectModel> loadObjModel(const string &filePath);
    static void processVertices(shared_ptr<ObjectModel> pModel,
                                const vector<string>& pieces);
private:
    static void init();

    static unordered_map<string, unsigned int> vertexCompositionIndices;
};

} // Base
} // GameEngine
} // ForlornMelody


#endif //MUSICBOX_OBJLOADER_H
