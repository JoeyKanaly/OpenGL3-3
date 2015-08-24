#version 330 core

in VS_OUT
{
	vec2 texture;
} fs_in;

uniform sampler2D texture_specular1;

out vec4 color;

void main()
{
	color = vec4(texture(texture_specular1, fs_in.texture));
}