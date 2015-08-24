#version 330 core

layout (location = 0) in vec3 position;
layout (location = 2) in vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out VS_OUT
{
	vec2 texture;
} vs_out;

void main()
{
	gl_Position = proj * view * model * vec4(position, 1.0f);
	vs_out.texture = texCoords;
}