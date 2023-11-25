#version 460 core

out vec4 oColor;

in vec2 ioUv;

uniform sampler2D uTexture;

void main()
{
    oColor = texture(uTexture, ioUv);
}