#version 460 core

layout (location = 1) in vec3 aPosition;
layout (location = 2) in vec2 aUv;

out vec2 ioUv;

uniform mat4 uProjection;

void main()
{
    ioUv = aUv;
    gl_Position = uProjection * vec4(aPosition.x, aPosition.y, -1, 1);
}