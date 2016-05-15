//
// Created by Glenn Jacob on 28/03/16.
//

#include "TerrainShader.h"
#include "Constants.h"
#include "Camera.h"
#include "Light.h"


namespace ForlornMelody {
namespace GameEngine {
namespace Base {


TerrainShader::TerrainShader()
{
    this->compileShaders("./Resources/Shaders/terrainVertexShader.vert",
                         "./Resources/Shaders/terrainFragmentShader.frag");
    this->linkShaders();
    this->setAttributesCount(4);
    this->loadAllUniformVariableLocations();
}

TerrainShader::~TerrainShader()
{
}

void TerrainShader::loadAllUniformVariableLocations()
{
    this->uniformVarLocations[TEXTURE_SAMPLER] = this->getUniformLocation(TEXTURE_SAMPLER);
    this->uniformVarLocations[TRANSFORMATION_MATRIX] =
        this->getUniformLocation(TRANSFORMATION_MATRIX);
    this->uniformVarLocations[PROJECTION_MATRIX] = this->getUniformLocation(PROJECTION_MATRIX);
    this->uniformVarLocations[VIEW_MATRIX] = this->getUniformLocation(VIEW_MATRIX);
    this->uniformVarLocations[LIGHT_POSITION] = this->getUniformLocation(LIGHT_POSITION);
    this->uniformVarLocations[LIGHT_COLOR] = this->getUniformLocation(LIGHT_COLOR);
    this->uniformVarLocations[REFLECTIVITY] = this->getUniformLocation(REFLECTIVITY);
    this->uniformVarLocations[SHINE_DAMPER] = this->getUniformLocation(SHINE_DAMPER);
}

void TerrainShader::loadProjectionMatrix(const mat4& projection)
{
    this->setUniformMatrix4(PROJECTION_MATRIX, projection);
}

void TerrainShader::loadViewMatrix(const Camera& camera)
{
    this->setUniformMatrix4(VIEW_MATRIX, camera.createViewMatrix());
}

void TerrainShader::loadLightParameters(const Light& light)
{
    this->setUniformVector3(LIGHT_POSITION, light.getPosition());
    this->setUniformVector3(LIGHT_COLOR, light.getColor());
}

void TerrainShader::loadShineVariables(const float reflectivity, const float shineDamper)
{
    this->setUniformFloat(REFLECTIVITY, reflectivity);
    this->setUniformFloat(SHINE_DAMPER, shineDamper);
}
}
}
}

