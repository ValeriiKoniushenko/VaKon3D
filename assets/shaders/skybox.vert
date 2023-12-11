#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 ioUv;

uniform mat4 uProjectionView;
uniform mat4 uModel;

void main()
{
    ioUv = aPos;
    gl_Position = uProjectionView * uModel * vec4(aPos, 1.0);
}