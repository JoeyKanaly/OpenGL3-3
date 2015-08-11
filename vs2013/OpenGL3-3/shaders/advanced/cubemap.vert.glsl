#version 330 core

layout (location = 0) in vec3 position;

out vec3 textureDir;

uniform mat4 view;
uniform mat4 proj;

void main()
{
	vec4 pos = proj * view * vec4(position, 1.0f);
	gl_Position = pos.xyww;
	textureDir = position;
}