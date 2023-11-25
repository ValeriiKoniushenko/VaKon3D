#version 460 core

layout (location = 1) in vec2 aPosition;
layout (location = 2) in vec2 aUv;

void main()
{
    gl_Position = vec4(aPosition.x, aPosition.y, 0, 1);
}