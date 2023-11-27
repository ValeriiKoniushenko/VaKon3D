#version 460 core

layout (location = 1) in vec3 aPosition;
layout (location = 2) in vec2 aUv;
layout (location = 2) in vec3 aNormal;

out vec2 ioUv;

uniform mat4 uProjection;
uniform mat4 uModel;
uniform mat4 uView;

void main()
{
    ioUv = aUv;
    gl_Position = uProjection * uView * uModel * vec4(aPosition, 1);
}