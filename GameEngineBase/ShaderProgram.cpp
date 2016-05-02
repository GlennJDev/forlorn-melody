//
// Created by Glenn Jacob on 28/12/15.
//

#include "ShaderProgram.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>


using std::cerr;
using std::endl;
using std::ifstream;
using std::vector;

namespace ForlornMelody {
namespace GameEngine {
namespace Base {

ShaderProgram::ShaderProgram()
{

}

ShaderProgram::~ShaderProgram()
{
    glDetachShader(this->programId, this->vertexShaderId);
    glDetachShader(this->programId, this->fragmentShaderId);
    glDeleteShader(this->vertexShaderId);
    glDeleteShader(this->fragmentShaderId);
    glDeleteProgram(this->programId);
}


void ShaderProgram::compileShaders(const string &vertexShaderFilePath, const string &fragmentShaderFilePath)
{
    this->programId = glCreateProgram();
    this->vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    if (this->vertexShaderId == 0) {
        cerr << "Failed to created the vertex shader" << std::endl;
        exit(EXIT_FAILURE);
    }

    this->fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    if (this->fragmentShaderId == 0) {
        cerr << "Failed to created the fragment shader" << std::endl;
        exit(EXIT_FAILURE);
    }

    this->compileShader(vertexShaderFilePath, this->vertexShaderId);
    this->compileShader(fragmentShaderFilePath, this->fragmentShaderId);
}

void ShaderProgram::linkShaders()
{
    //Attach our shaders to our program
    glAttachShader(this->programId, this->vertexShaderId);
    glAttachShader(this->programId, this->fragmentShaderId);

    //Link our program
    glLinkProgram(this->programId);

    //Note the different functions here: glGetProgram* instead of glGetShader*.
    GLint isLinked = 0;
    glGetProgramiv(this->programId, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE) {
        GLint logLength = 0;
        glGetProgramiv(this->programId, GL_INFO_LOG_LENGTH, &logLength);

        //The logLength includes the NULL character
        std::vector<char> errorLog(logLength);
        glGetProgramInfoLog(this->programId, logLength, &logLength, &errorLog[0]);

        //We don't need the this->programId anymore.
        glDeleteProgram(this->programId);
        //Don't leak shaders either.
        glDeleteShader(this->vertexShaderId);
        glDeleteShader(this->fragmentShaderId);

        //Use the infoLog as you see fit.
        std::cerr << &(errorLog[0]) << std::endl;
        cerr << "Program failed to link" << endl;
    }

    //Always detach shaders after a successful link.
    glDetachShader(this->programId, this->vertexShaderId);
    glDetachShader(this->programId, this->fragmentShaderId);

    glDeleteShader(this->vertexShaderId);
    glDeleteShader(this->fragmentShaderId);
}

void ShaderProgram::use()
{
    glUseProgram(this->programId);
}

void ShaderProgram::unuse()
{
    glUseProgram(0);
}

GLint ShaderProgram::getUniformLocation(const string &uniformName)
{
    GLint location = GL_INVALID_INDEX;
    if (this->uniformVarLocations.count(uniformName) == 0) {
        location = glGetUniformLocation(this->programId, uniformName.c_str());
        if (location == GL_INVALID_INDEX) {
            cerr << "Uniform " << uniformName << " not found in shader!" << endl;
        }
    } else {
        location = this->uniformVarLocations[uniformName];
    }

    return location;
}

void ShaderProgram::compileShader(const string &filePath, const GLuint shaderId)
{
    auto vertexFile = ifstream(filePath);
    if(vertexFile.fail()) {
        perror(filePath.c_str());
        cerr << "Failed to open " << filePath << endl;
        exit(EXIT_FAILURE);
    }

    auto fileContentsStream = std::stringstream();
    string line;
    while (getline(vertexFile, line)) {
        // Belangrijk van het op deze manier te doen!
        fileContentsStream << std::endl << line;
    }

    vertexFile.close();

    // Source van de shader doorgeven aan openGL
    auto fileContents = fileContentsStream.str();
    auto contents = fileContents.c_str();
    // De source zit in 1 string
    glShaderSource(shaderId, 1, &contents, nullptr);

    glCompileShader(shaderId);
    GLint success = 0;
    GLint logLength;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);

        // The maxLength include the NULL character
        auto errorLog = vector<char>(logLength);
        glGetShaderInfoLog(shaderId, logLength, &maxLength, errorLog.data());

        // Provide the infolog in whatever manor you deem best.
        // Exit with failure.
        glDeleteShader(shaderId); // Don't leak the shader

        std::cerr << &(errorLog[0]) << std::endl;
        cerr << "Shader failed to compile. location: " << filePath << endl;
        exit(EXIT_FAILURE);
    }
}

void ShaderProgram::setAttributesCount(const int attributesCount)
{
    this->numAttributes = attributesCount;
}

void ShaderProgram::enableAttributes()
{
    // Tell opengl that we want to use these attribute arrays
    // from our shader program. 0, 1 ,2, ... are the locations of
    // shader input variables.
    for (int i = 0; i < this->numAttributes; ++i) {
        glEnableVertexAttribArray(i);
    }
}

void ShaderProgram::disableAttributes()
{
    for (int i = 0; i < this->numAttributes; ++i) {
        glDisableVertexAttribArray(i);
    }
}

void ShaderProgram::setUniformFloat(const string &uniformName, const GLfloat value)
{
    auto location = this->getUniformLocation(uniformName);
    glUniform1f(location, value);
}

void ShaderProgram::setUniformInt(const string &uniformName, const GLint value)
{
    auto location = this->getUniformLocation(uniformName);
    glUniform1i(location, value);
}

void ShaderProgram::setUniformBool(const string &uniformName, const GLboolean value)
{
    auto location = this->getUniformLocation(uniformName);
    glUniform1i(location, value);
}

void ShaderProgram::setUniformVector3(const string &uniformName, const vec3 &value)
{
    auto location = this->getUniformLocation(uniformName);
    glUniform3f(location, value[0], value[1], value[2]);
}

void ShaderProgram::setUniformMatrix4(const string &uniformName, const mat4 &value)
{
    auto location = this->getUniformLocation(uniformName);
    glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
    // OF: #include <glm/gtc/type_ptr.hpp> EN glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderProgram::enableAlphaBlending() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ShaderProgram::disableAplphaBlending() {
    glDisable(GL_BLEND);
}

void ShaderProgram::enableDepthTesting()
{
    glEnable(GL_DEPTH_TEST);
}

void ShaderProgram::disableDebptTesting()
{
    glDisable(GL_DEPTH_TEST);
}

void ShaderProgram::enableCullFace()
{
    glEnable(GL_CULL_FACE);
    // Do not render the back faces of the model
    glCullFace(GL_BACK);
}

void ShaderProgram::disableCullFace()
{
    glDisable(GL_CULL_FACE);
}

} // Base
} // GameEngine
} // ForlornMelody

