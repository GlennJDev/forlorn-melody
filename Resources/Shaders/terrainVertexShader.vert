#version 330 core
// Vertex shader

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec4 vertexColor;
layout(location = 2) in vec2 vertexUV;
layout(location = 3) in vec3 vertexNormal;

out vec2 fragmentPosition;
out vec4 fragmentColor;
out vec2 fragmentUV;
out vec3 surfaceNormal;
out vec3 toLightVector;
out vec3 toCameraVector;

uniform mat4 transformationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform vec3 lightPosition;

void main(){

    vec4 worldPosition = transformationMatrix * vec4(vertexPosition, 1.0);
    vec4 posRelativeToCam = viewMatrix * worldPosition;
    gl_Position = projectionMatrix * posRelativeToCam;
    fragmentColor = vertexColor;
    fragmentUV = vertexUV;

    surfaceNormal = (transformationMatrix * vec4(vertexNormal, 0.0)).xyz;
    toLightVector = lightPosition - worldPosition.xyz;
    toCameraVector = (inverse(viewMatrix) * vec4(0.0,0.0,0.0,1.0)).xyz;
}