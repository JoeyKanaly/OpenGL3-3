#version 330 core

in vec3 outColor;
in vec2 texCoords;

out vec4 color;

uniform sampler2D myTexture1;
uniform sampler2D myTexture2;

void main()
{
	color = mix(texture(myTexture1, texCoords), texture(myTexture2, texCoords), 0.2f);
}
