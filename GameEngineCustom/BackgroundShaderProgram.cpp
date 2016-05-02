//
// Created by Glenn Jacob on 24/04/16.
//

#include <GameEngineBase/Constants.h>
#include <GameEngineBase/Camera.h>
#include <GameEngineBase/Light.h>
#include "BackgroundShaderProgram.h"

namespace ForlornMelody {
namespace GameEngine {
using namespace Base;
namespace Custom {

BackgroundShaderProgram::BackgroundShaderProgram()
{
    this->compileShaders("./Resources/Shaders/vertexShaderBackground.vert", "./Resources/Shaders/fragmentShaderBackground.frag");
    this->linkShaders();
    this->setAttributesCount(4);
    this->enableAlphaBlending();
    //this->enableDepthTesting();
    //this->enableCullFace();
    this->loadAllUniformVariableLocations();
}

BackgroundShaderProgram::~BackgroundShaderProgram()
{
}

void BackgroundShaderProgram::loadAllUniformVariableLocations()
{
    this->uniformVarLocations[TEXTURE_SAMPLER] = this->getUniformLocation(TEXTURE_SAMPLER);
    this->uniformVarLocations[TRANSFORMATION_MATRIX] = this->getUniformLocation(TRANSFORMATION_MATRIX);
    //this->uniformVarLocations[PROJECTION_MATRIX] = this->getUniformLocation(PROJECTION_MATRIX);
//    this->uniformVarLocations[VIEW_MATRIX] = this->getUniformLocation(VIEW_MATRIX);
    //this->uniformVarLocations[USE_TEXTURE] = this->getUniformLocation(USE_TEXTURE);
//    this->uniformVarLocations[LIGHT_POSITION] = this->getUniformLocation(LIGHT_POSITION);
//    this->uniformVarLocations[LIGHT_COLOR] = this->getUniformLocation(LIGHT_COLOR);
    //this->uniformVarLocations[REFLECTIVITY] = this->getUniformLocation(REFLECTIVITY);
    //this->uniformVarLocations[SHINE_DAMPER] = this->getUniformLocation(SHINE_DAMPER);
    //this->uniformVarLocations[SKY_COLOR] = this->getUniformLocation(SKY_COLOR);
}

void BackgroundShaderProgram::loadProjectionMatrix(const mat4 &projection)
{
    this->setUniformMatrix4(PROJECTION_MATRIX, projection);
}

void BackgroundShaderProgram::loadViewMatrix(const Camera &camera)
{
    this->setUniformMatrix4(VIEW_MATRIX, camera.createViewMatrix());
}

void BackgroundShaderProgram::loadLightParameters(const Light& light)
{
    this->setUniformVector3(LIGHT_POSITION, light.getPosition());
    this->setUniformVector3(LIGHT_COLOR, light.getColor());
}

void BackgroundShaderProgram::loadShineVariables(const float reflectivity, const float shineDamper)
{
    this->setUniformFloat(REFLECTIVITY, reflectivity);
    this->setUniformFloat(SHINE_DAMPER, shineDamper);
}

} // Custom
} // GameEngine
} // ForlornMelody



