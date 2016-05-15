//
// Created by Glenn Jacob on 28/03/16.
//

#ifndef MUSICBOX_TERRAINSHADER_H
#define MUSICBOX_TERRAINSHADER_H

#include "ShaderProgram.h"


namespace ForlornMelody {
namespace GameEngine {
namespace Base {

class TerrainShader: public ShaderProgram
{
public:
    TerrainShader();
    virtual ~TerrainShader();

    void loadProjectionMatrix(const mat4& projection) override;
    virtual void loadViewMatrix(const Camera& camera) override;
    virtual void loadLightParameters(const Light& light) override;

    virtual void loadShineVariables(const float reflectivity, const float shineDamper) override;
private:
    virtual void loadAllUniformVariableLocations() override;
};

}
}
}



#endif //MUSICBOX_TERRAINSHADER_H
