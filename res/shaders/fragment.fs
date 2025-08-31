#version 300 es

precision highp float;

layout(location = 0) out vec3 gColor;
layout(location = 1) out vec3 gNormal;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;
in vec3 VertexColor;

uniform sampler2D image;

void main()
{
	gColor = vec3(1.0, 0.0, 1.0);
	vec3 normal = normalize(Normal);
	gNormal = normal * 0.5 + 0.5;
	gColor = texture(image, TexCoord).rgb;
}