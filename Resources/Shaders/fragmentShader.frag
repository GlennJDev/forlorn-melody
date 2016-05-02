#version 330 core
// Fragment shader

in vec4 fragmentPosition;
in vec4 fragmentColor;
in vec2 fragmentUV;
in vec3 surfaceNormal;
in vec3 toLightVector;
in vec3 toCameraVector;
in float visibilty;

out vec4 outColor;

uniform sampler2D textureSampler;
uniform bool useTexture;
uniform vec3 lightColor;
uniform float reflectivity;
uniform float shineDamper;

void main() {

    vec3 unitNormal = normalize(surfaceNormal);
    vec3 unitLightVector = normalize(toLightVector);

    float nDot1 = dot(unitNormal, unitLightVector);
    float brightness = max(nDot1, 0.2);
    vec3 diffuse = brightness * lightColor;

    // Color blending
    vec4 tempColor;
    if (useTexture) {
        vec4 textureColor = texture(textureSampler, fragmentUV);
        tempColor = textureColor;
    } else {
        tempColor = fragmentColor;
    }

    if (tempColor.a < 0.5) {
        discard;
    }

    vec3 unitCameraVector = normalize(toCameraVector);
    vec3 lightDirection = -unitLightVector;
    vec3 reflectedLightDirection = reflect(lightDirection, unitNormal);

    float specularFactor =  dot(reflectedLightDirection, unitCameraVector);
    specularFactor = max(specularFactor, 0.0);
    float dampedFactor =  pow(specularFactor, shineDamper);
    vec3 finalSpecular = dampedFactor * reflectivity * lightColor;

    outColor = vec4(diffuse, 1.0) * tempColor + vec4(finalSpecular, 1.0);
}