//
// Created by Glenn Jacob on 26/12/15.
//

#ifndef MUSICBOX_RESOURCECACHE_H
#define MUSICBOX_RESOURCECACHE_H

#include <string>
#include <unordered_map>
#include <memory>
#include "GraphicsUtils.h"

using std::string;
using std::unordered_map;
using std::shared_ptr;

namespace ForlornMelody {
namespace GameEngine {
namespace Base {

class ObjectModel;

class ResourceCache
{
public:
    ResourceCache();
    virtual ~ResourceCache();

    GLTexture loadTexture(const string& texturePath, const bool hasTransparency = false);
    std::shared_ptr<ObjectModel> loadModel(const string& modelPath);
private:
    unordered_map<string, GLTexture> textures;
    unordered_map<string, shared_ptr<ObjectModel>> models;
};

} // Base
}
}




#endif //MUSICBOX_RESOURCECACHE_H
