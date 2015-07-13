#version 330 core

in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

uniform sampler2D myTexture1;
uniform sampler2D myTexture2;


void main()
{
	//color = vec4(ourColor, 1.0f);
	color = mix(texture(myTexture1, TexCoord), texture(myTexture2, TexCoord), 0.2f);
}