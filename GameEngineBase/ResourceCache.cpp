//
// Created by Glenn Jacob on 26/12/15.
//

#include "ResourceCache.h"
#include <iostream>
#include "ImageLoader.h"
#include "ObjectModelLoader.h"
#include "ObjectModel.h"

using std::cout;
using std::endl;

namespace ForlornMelody {
namespace GameEngine {
namespace Base {

ResourceCache::ResourceCache()
{
}

ResourceCache::~ResourceCache()
{
    for (auto&& texturePair : this->textures) {
        glDeleteTextures(1, &(texturePair.second.id));
    }
}

GLTexture ResourceCache::loadTexture(const string& texturePath, const bool hasTransparency /* = false*/)
{
    auto found = this->textures.count(texturePath);
    if (found == 0) {
        auto newTexture = ImageLoader::loadPNG(texturePath);
        newTexture.hasTransparency = hasTransparency;

        auto keyVal = std::make_pair(texturePath, newTexture);
        this->textures.insert(keyVal);

        cout << "New texture at " << texturePath << " loaded" << endl;

        return newTexture;
    }

    return textures[texturePath];
}

shared_ptr<ObjectModel> ResourceCache::loadModel(const string& modelPath)
{
    auto found = this->models.count(modelPath);
    if (found == 0) {
        auto newModel = ObjectModelLoader::loadObjModel(modelPath);

        auto keyVal = std::make_pair(modelPath, newModel);
        this->models.insert(keyVal);

        cout << "New model at " << modelPath << " loaded" << endl;

        return newModel;
    }
    return models[modelPath];
}

} // Base
}
}



