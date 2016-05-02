//
// Created by Glenn Jacob on 23/12/15.
//

#ifndef MUSICBOX_GLSLPROGRAM_H
#define MUSICBOX_GLSLPROGRAM_H

#include "ShaderProgram.h"

namespace ForlornMelody {
namespace GameEngine {
namespace Base {

class StaticShaderProgram: public ShaderProgram
{
public:
    StaticShaderProgram();
    virtual ~StaticShaderProgram();

    void loadProjectionMatrix(const mat4& projection) override;
    virtual void loadViewMatrix(const Camera& camera) override;
    virtual void loadLightParameters(const Light& light) override;

    virtual void loadShineVariables(const float reflectivity, const float shineDamper) override;
private:
    virtual void loadAllUniformVariableLocations() override;
};
} // Base
} // GameEngine
} // ForlornMelody




#endif //MUSICBOX_GLSLPROGRAM_H
