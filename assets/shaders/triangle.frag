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
uniform vec3 uAmbientLightColor;
uniform vec3 uAmbientLightDirection;
uniform float uAmbientLightMaxDark;
uniform vec3 uSpecularColor;
uniform float uSpecularIntensity;
uniform int uSpecularPow;

void main()
{
    vec3 normal = normalize(ioNormal);

    float diffuseLight = max(dot(normal, uAmbientLightDirection), uAmbientLightMaxDark);

    vec3 surfaceToLight = normalize(ioSurfaceToLight);
    vec3 surfaceToView = normalize(ioSurfaceToView);
    vec3 halfVector = normalize(surfaceToView + surfaceToLight);
    float specularLight = max(pow(dot(normal, halfVector), uSpecularPow) * uSpecularIntensity, 0.f);

    vec2 uv = vec2(ioTextureRectSize.x + ioUv.x, ioTextureRectSize.y + ioUv.y);

    oColor = texture(uTexture, uv) * vec4(uAmbientLightColor, 1.f);
    oColor.rgb *= diffuseLight;
    oColor.rgb += specularLight * uSpecularColor;
}