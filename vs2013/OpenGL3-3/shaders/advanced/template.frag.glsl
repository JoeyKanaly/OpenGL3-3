#version 330 core

in vec2 TextureCoords;

uniform sampler2D tex;

out vec4 color;

void main()
{
	vec4 texColor = texture(tex, TextureCoords);
	if(texColor.a < 0.5)
		discard;
	//color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	color = texColor;
}