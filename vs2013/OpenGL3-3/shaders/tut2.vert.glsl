#version 330 core
#extension GL_ARB_seperate_shader_objects : enable
layout (location = 0) in vec3 position;

void main()
{
	gl_Position = vec4(position.x, position.y, position.z, 1.0);
}