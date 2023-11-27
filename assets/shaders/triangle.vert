#version 460 core

layout (location = 1) in vec3 aPosition;
layout (location = 2) in vec2 aUv;
layout (location = 2) in vec3 aNormal;

out vec2 ioUv;

uniform mat4 uModel;
uniform mat4 uProjectionAndView;

void main()
{
    ioUv = aUv;
    gl_Position = uProjectionAndView * uModel * vec4(aPosition, 1);
}