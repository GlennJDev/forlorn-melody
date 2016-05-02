//
// Created by Glenn Jacob on 26/12/15.
//

#ifndef MUSICBOX_IMAGELOADER_H
#define MUSICBOX_IMAGELOADER_H

#include <string>
#include "GraphicsUtils.h"

using std::string;

namespace ForlornMelody {
namespace GameEngine {
namespace Base {

class ImageLoader
{
public:
    static GLTexture loadPNG(const string& filePath);
private:
};
}
}
}





#endif //MUSICBOX_IMAGELOADER_H
