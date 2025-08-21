#version 300 es

precision mediump float;

in vec3 vColors;
out vec4 fragColor;

void main()
{
    fragColor = vec4(vColors, 1.0);
}