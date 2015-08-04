#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

//out vec3 Normals;
out vec2 TextureCoords;

void main()
{
	gl_Position = proj * view *  model * vec4(position, 1.0f);
	//gl_Position = vec4(position, 1.0f);
//	Normals = normal;
	TextureCoords = texcoords;
}