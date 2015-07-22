#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 norm;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 FragPos;
out vec3 normal;
out vec3 newLightPosition;
void main()
{
	gl_Position = proj * view * model * vec4(position, 1.0f);
	FragPos = vec3(model * vec4(position, 1.0f));

	//Fine for learning purposes but do this on the CPU before sending it to the GPU
	normal = mat3(transpose(inverse(model))) * norm;
}