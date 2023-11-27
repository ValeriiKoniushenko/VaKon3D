#version 460 core

layout (location = 1) in vec2 aPosition;
layout (location = 2) in vec2 aUv;

out vec2 ioUv;

uniform mat4 uProjection;
uniform mat4 uModel;

void main()
{
    ioUv = aUv;
    gl_Position = uProjection * uModel * vec4(aPosition.x, aPosition.y, -1, 1);
}