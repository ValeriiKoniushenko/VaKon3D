#version 460 core

out vec4 oColor;

in vec2 ioUv;
in vec3 ioNormal;
in vec3 ioFragmentPosition;

uniform sampler2D uTexture;
uniform vec3 uAmbientLightColor;
uniform vec3 uAmbientLightDirection;
uniform float uAmbientLightMaxDark;

void main()
{
    vec3 normal = normalize(ioNormal);

    float diffuseLight = max(dot(normal, uAmbientLightDirection), uAmbientLightMaxDark);

    oColor = texture(uTexture, ioUv);
    oColor.rgb *= diffuseLight * uAmbientLightColor;
}