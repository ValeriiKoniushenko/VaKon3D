#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 uProjectionView;

out vec4 ioFragColorPos;

void main()
{
    ioFragColorPos = vec4(aPos, 1.0);
    gl_Position = uProjectionView * vec4(aPos, 1.0);
}