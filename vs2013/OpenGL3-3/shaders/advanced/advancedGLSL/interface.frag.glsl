#version 330 core

in VS_OUT
{
	vec2 TexCoords;
} fs_in;

uniform smapler2D tex;

out vec4 color;

void main()
{
	color = texture(tex, fs_in.TexCoords);
}