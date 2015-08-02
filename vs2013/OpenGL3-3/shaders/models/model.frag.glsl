#version 330 core

in vec2 TexCoords;
in vec3 inColor;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform float time;

out vec4 color;

void main()
{
	color = vec4(texture(texture_specular1, TexCoords));
	//color = vec4(sin(time), 1.0f, 0.4f, 1.0f);
	//color = vec4(1.0f, 0.5f, 0.3, 1.0f);
	//color = vec4(inColor, 1.0f);
}