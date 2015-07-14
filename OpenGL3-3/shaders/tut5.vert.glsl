#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texture;

out vec3 outColor;
out vec2 TexCoord;

uniform mat4 trans;
uniform float time;
void main()
{
	//gl_Position = trans * vec4(position.x * ((sin(time)/2)+0.5f),
	 //position.y * ((sin(time)/2)+0.5f), position.z, 1.0f);
	gl_Position = trans * vec4(position, 1.0f);
	outColor = color;
	TexCoord = vec2(texture.x, 1.0f - texture.y);
}