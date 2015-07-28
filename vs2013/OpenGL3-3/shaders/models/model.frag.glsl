#version 330 core

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform float time;

out vec4 color;

void main()
{
	color = vec4(texture(texture_diffuse1, TexCoords));
	//color = vec4(sin(time), 1.0f, 0.4f, 1.0f);
}