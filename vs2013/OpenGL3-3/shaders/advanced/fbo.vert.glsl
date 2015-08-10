#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 tex;

out vec2 texCoords;

void main()
{
	gl_Position = vec4(position.xy, 0.0f, 1.0f);
	texCoords = tex;
}