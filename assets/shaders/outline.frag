#version 330 core
out vec4 FragColor;

uniform vec4 uOutlineColor;

void main()
{
    FragColor = uOutlineColor;
}