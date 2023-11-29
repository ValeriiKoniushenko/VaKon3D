#version 460 core

out vec4 oColor;

in vec2 ioUv;
in vec3 ioNormal;

struct AmbientLight
{
    vec3 lightColor;
};

struct Light
{
    AmbientLight ambientLight;
} light;

uniform sampler2D uTexture;
uniform vec3 uAmbientLightLightColor;

void main()
{
    light.ambientLight.lightColor = uAmbientLightLightColor;

    oColor = texture(uTexture, ioUv) * vec4(light.ambientLight.lightColor, 1.f);
}