#version 460 core

layout (location = 1) in vec3 aPosition;
layout (location = 2) in vec2 aUv;
layout (location = 3) in vec3 aNormal;
layout (location = 4) in vec2 aTextureRectSize;

out vec2 ioUv;
out vec2 ioTextureRectSize;
out vec3 ioNormal;
out vec3 ioFragmentPosition;
out vec3 ioSurfaceToView;
out vec3 ioSurfaceToLight;

uniform vec2 uAtlasSize;
uniform mat4 uModel;
uniform mat4 uProjectionAndView;
uniform vec3 uSpecularPosition;
uniform vec3 uViewPosition;

void main()
{
    ioNormal = mat3(transpose(inverse(uModel))) * aNormal;

    ioUv = uAtlasSize * aUv / uAtlasSize;//calculate texture position at the atlas by according to atlas size
    ioTextureRectSize = aTextureRectSize / uAtlasSize;//calculate texture size by according to atlas size

    ioFragmentPosition = vec3(uModel * vec4(aPosition, 1.f));
    ioSurfaceToLight = uSpecularPosition - (uModel * vec4(aPosition, 1.f)).xyz;
    ioSurfaceToView = uViewPosition - (uModel * vec4(aPosition, 1.f)).xyz;

    gl_Position = uProjectionAndView * uModel * vec4(aPosition, 1.f);
}