#version 460 core

out vec4 oColor;

in vec2 ioUv;

uniform sampler2D uTexture;

void main()
{
    vec4 outColor = texture(uTexture, ioUv);
    oColor = outColor;
}