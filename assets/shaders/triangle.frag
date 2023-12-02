#version 460 core

out vec4 oColor;

in vec2 ioUv;
in vec2 ioTextureRectSize;
in vec3 ioNormal;
in vec3 ioFragmentPosition;
in vec3 ioSurfaceToView;
in vec3 ioSurfaceToLight;

uniform vec3 uViewPosition;
uniform sampler2D uTexture;
uniform sampler2D uSpecularTexture;
uniform bool uIsHasSpecularTexture;
uniform vec3 uAmbientLightColor;
uniform vec3 uAmbientLightDirection;
uniform float uAmbientLightMaxDark;
uniform vec3 uSpecularColor;
uniform float uSpecularIntensity;
uniform int uSpecularPow;
uniform vec4 uFogColor;
uniform float uFogMinDistance;
uniform float uFogMaxDistance;

float getFogFactor(float d)
{
    if (d >= uFogMaxDistance)
    {
        return 1;
    }
    if (d <= uFogMinDistance)
    {
        return 0;
    }

    return 1 - (uFogMaxDistance - d) / (uFogMaxDistance - uFogMinDistance);
}

vec4 calcFog(vec4 color)
{
    float d = distance(uViewPosition, ioFragmentPosition);
    return mix(color, uFogColor, getFogFactor(d));
}

void main()
{
    vec3 normal = normalize(ioNormal);

    float diffuseLight = max(dot(normal, uAmbientLightDirection), uAmbientLightMaxDark);

    vec3 surfaceToLight = normalize(ioSurfaceToLight);
    vec3 surfaceToView = normalize(ioSurfaceToView);
    vec3 halfVector = normalize(surfaceToView + surfaceToLight);
    float specularPow = uSpecularPow;
    if (uIsHasSpecularTexture)
    {
        specularPow = mix(512.f, 4096.f, texture(uSpecularTexture, ioUv).r);
    }
    float specularLight = max(pow(dot(normal, halfVector), specularPow) * uSpecularIntensity, 0.f);

    vec2 uv = vec2(ioTextureRectSize.x + ioUv.x, ioTextureRectSize.y + ioUv.y);

    oColor = texture(uTexture, uv) * vec4(uAmbientLightColor, 1.f);
    oColor.rgb *= diffuseLight;
    oColor.rgb += specularLight * uSpecularColor;
    oColor = calcFog(oColor);
}