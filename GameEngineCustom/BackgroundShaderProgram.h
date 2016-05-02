//
// Created by Glenn Jacob on 24/04/16.
//

#ifndef FORLORNMELODY_BACKGROUNDSHADERPROGRAM_H
#define FORLORNMELODY_BACKGROUNDSHADERPROGRAM_H

#include <GameEngineBase/ShaderProgram.h>

namespace ForlornMelody {
namespace GameEngine {

using Base::Camera;
using Base::Light;

namespace Custom {

class BackgroundShaderProgram : public Base::ShaderProgram
{
public:
    BackgroundShaderProgram();
    virtual ~BackgroundShaderProgram();

    virtual void loadProjectionMatrix(const mat4& projection) override;
    virtual void loadViewMatrix(const Camera &camera) override;
    virtual void loadLightParameters(const Light& light) override;

    virtual void loadShineVariables(const float reflectivity, const float shineDamper) override;
private:
    virtual void loadAllUniformVariableLocations() override;
};

} // Custom
} // GameEngine
} // ForlornMelody




#endif //FORLORNMELODY_BACKGROUNDSHADERPROGRAM_H
