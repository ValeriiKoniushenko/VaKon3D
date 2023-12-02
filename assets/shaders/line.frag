#version 330 core
out vec4 FragColor;

uniform vec4 uLineColor;
uniform vec3 uViewPosition;

in vec4 ioFragColorPos;

vec4 uFogColor = vec4(0.1, 0.1, 0.1, 0.0);
float uFogMinDistance = 500;
float uFogMaxDistance = 5000;

float getFogFactor(float d)
{
    if (d >= uFogMaxDistance)
    {
        return 1;
    }
    if (d <= uFogMinDistance)
    {
        return 0;
    }

    return 1 - (uFogMaxDistance - d) / (uFogMaxDistance - uFogMinDistance);
}

vec4 calcFog(vec4 color)
{
    float d = distance(vec4(uViewPosition, 1.f), ioFragColorPos);
    return mix(color, uFogColor, getFogFactor(d));
}

void main()
{
    FragColor = calcFog(uLineColor);
}