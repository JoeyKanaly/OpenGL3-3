#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 normal;
out vec3 fragPos;
out vec2 TexCoords;

void main()
{
	gl_Position = proj * view * model * vec4(position, 1.0f);

	fragPos = vec3(model * vec4(position, 1.0f));
	normal = mat3(transpose(inverse(model))) * norm;
	TexCoords = texCoords;
}