//
// Created by Glenn Jacob on 26/12/15.
//

#include "ImageLoader.h"
#include <vector>
#include <GL/glew.h>
#include <SOIL.h>
#include <iostream>
#include "Logger.h"

using std::cerr;
using std::endl;

namespace ForlornMelody {
namespace GameEngine {
namespace Base {

GLTexture ImageLoader::loadPNG(const string& filePath)
{
    //Create a GLTexture and initialize all its fields to 0
    GLTexture texture = {};

    int width, height, channels;
    unsigned char* image = SOIL_load_image(filePath.c_str(), &width, &height, &channels, SOIL_LOAD_RGBA);
    if (image == nullptr) {
        Logger::logError("ObjectModelLoader::loadObjModel", "filepath (", filePath, ") does not exist");
        exit(EXIT_FAILURE);
    }

    //Generate the openGL texture object
    glGenTextures(1, &(texture.id));

    //Bind the texture object
    glBindTexture(GL_TEXTURE_2D, texture.id);
    //Upload the pixels to the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    //Set some texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    //Generate the mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);

    //Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    SOIL_free_image_data(image);

    texture.width = width;
    texture.height = height;

    //Return a copy of the texture data
    return texture;
}
}
}
}




