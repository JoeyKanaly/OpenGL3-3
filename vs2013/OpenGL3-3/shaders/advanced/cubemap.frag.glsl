#version 330 core

in vec3 textureDir;
uniform samplerCube cubemap;

out vec4 color;

void main()
{
	color = texture(cubemap, textureDir);
}