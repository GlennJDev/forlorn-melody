#version 330 core
// Vertex shader

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec4 vertexColor;
layout(location = 2) in vec2 vertexUV;
layout(location = 3) in vec3 vertexNormal;

out vec2 fragmentPosition;
out vec4 fragmentColor;
out vec2 fragmentUV;

uniform mat4 transformationMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main(){

    vec4 worldPosition = transformationMatrix * vec4(vertexPosition, 1.0);
    vec4 posRelativeToCam = viewMatrix * worldPosition;
    gl_Position = projectionMatrix * posRelativeToCam;
    fragmentColor = vertexColor;
    fragmentUV = vertexUV;
}