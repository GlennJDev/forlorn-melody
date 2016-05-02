//
// Created by Glenn Jacob on 28/12/15.
//

#ifndef MUSICBOX_SHADERPROGRAM_H
#define MUSICBOX_SHADERPROGRAM_H

#include <string>
#include <unordered_map>
#include <GL/glew.h>
#include <glm/glm.hpp>

using std::string;
using std::unordered_map;
using glm::vec3;
using glm::mat4;

namespace ForlornMelody {
namespace GameEngine {
namespace Base {

class Camera;
class Light;

class ShaderProgram
{
public:
    ShaderProgram();
    virtual ~ShaderProgram();

    void compileShaders(const string& vertexShaderFilePath, const string& fragmentShaderFilePath);
    void linkShaders();
    void use();
    void enableAttributes();
    void unuse();
    void disableAttributes();
    void setAttributesCount(const int attributesCount);

    GLint getUniformLocation(const string& uniformName);
    // Niet vergeten op te roepen
    virtual void loadAllUniformVariableLocations() = 0;
    virtual void loadProjectionMatrix(const mat4 &projection) = 0;
    virtual void loadViewMatrix(const Camera& camera) = 0;
    virtual void loadLightParameters(const Light& light) = 0;
    virtual void loadShineVariables(const float reflectivity, const float shineDamper) = 0;

    void setUniformFloat(const string& uniformName, const GLfloat value);
    void setUniformInt(const string& uniformName, const GLint value);
    void setUniformBool(const string& uniformName, const GLboolean value);
    void setUniformVector3(const string& uniformName, const vec3& value);
    void setUniformMatrix4(const string& uniformName, const mat4& value);

    // OpenGL settings
    void enableAlphaBlending();
    void disableAplphaBlending();
    void enableDepthTesting();
    void disableDebptTesting();
    // Render only what can be seen
    void enableCullFace();
    void disableCullFace();
protected:
    void compileShader(const string& filePath, const GLuint shaderId);

    GLuint programId = 0;
    GLuint vertexShaderId = 0;
    GLuint fragmentShaderId = 0;
    int numAttributes = 0;
    unordered_map<string, GLint> uniformVarLocations;
};

} // Base
} // GameEngine
} // ForlornMelody


#endif //MUSICBOX_SHADERPROGRAM_H
