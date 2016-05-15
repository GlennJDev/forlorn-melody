#version 330 core
// Fragment shader

in vec4 fragmentPosition;
in vec4 fragmentColor;
in vec2 fragmentUV;
in vec3 surfaceNormal;
in vec3 toLightVector;
in vec3 toCameraVector;

out vec4 outColor;

uniform sampler2D textureSampler;
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
    // Wanneer een uv coordinaat over 1 gaat start openGL gewoon weer van nul
    // Hierdoor kan textures gemakkelijk laten herhalen.
    vec2 repeatingUV = vec2(fragmentUV.x * 64, fragmentUV.y * 8);
    vec4 textureColor = texture(textureSampler, repeatingUV);

    vec3 unitCameraVector = normalize(toCameraVector);
    vec3 lightDirection = -unitLightVector;
    vec3 reflectedLightDirection = reflect(lightDirection, unitNormal);

    float specularFactor =  dot(reflectedLightDirection, unitCameraVector);
    specularFactor = max(specularFactor, 0.0);
    float dampedFactor =  pow(specularFactor, shineDamper);
    vec3 finalSpecular = dampedFactor * reflectivity * lightColor;

    outColor = vec4(diffuse, 1.0) * textureColor + vec4(finalSpecular, 1.0);
}