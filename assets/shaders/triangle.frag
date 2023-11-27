#version 460 core

out vec4 oColor;

in vec2 ioUv;

uniform sampler2D uTexture;

void main()
{
    oColor = vec4(1, 0, 0, 1);//texture(uTexture, ioUv);
}