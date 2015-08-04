#version 330 core

in vec2 TextureCoords;

uniform sampler2D tex;

out vec4 color;

void main()
{
	//color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	color = texture(tex, TextureCoords);
}