#version 460 core

layout (location = 1) in vec3 aPosition;
layout (location = 2) in vec2 aUv;
layout (location = 3) in vec3 aNormal;

out vec2 ioUv;
out vec3 ioNormal;
out vec3 ioFragmentPosition;

uniform mat4 uModel;
uniform mat4 uProjectionAndView;

void main()
{
    ioNormal = mat3(uModel) * aNormal;
    ioUv = aUv;
    ioFragmentPosition = mat3(uModel) * aPosition;

    gl_Position = uProjectionAndView * uModel * vec4(aPosition, 1.f);
}