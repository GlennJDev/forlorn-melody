#version 330 core
// Fragment shader

in vec4 fragmentPosition;
in vec4 fragmentColor;
in vec2 fragmentUV;

out vec4 outColor;

uniform sampler2D textureSampler;

void main() {
    // Color blending
    vec4 tempColor = texture(textureSampler, fragmentUV);
    outColor = tempColor;
}