#version 330 core

in vec2 texCoords;

uniform sampler2D myTexture;
uniform float time;

out vec4 color;

const float offset = 1.0f / 300.0f;

void main()
{
	vec2 offsets[9] = vec2[](
		vec2(-offset, offset),
		vec2(0.0f, offset),
		vec2(offset, offset),
		vec2(-offset, 0.0f),
		vec2(0.0f, 0.0f),
		vec2(offset, 0.0f),
		vec2(-offset, -offset),
		vec2(0.0f, -offset),
		vec2(offset, -offset)
	);

	float sinTime = (sin(time)/2) + .5;
	float cosTime = (cos(time)/2) + .5;

	float kernel[9] = float[](
		1,2,1,
		2,4,2,
		1,2,1
	);
	float sum = 0;
	for(int x = 0; x < 2; x++)
	{
		for(int i = 0; i < 9; i++)
		{
			if(x == 0)
				sum += kernel[i];
			else if(x == 1)
				kernel[i] /= sum;
		}
	}
	vec3 sampleTex[9];
	for (int i = 0; i < 9; i++)
	{
		sampleTex[i] = vec3(texture(myTexture, texCoords.st + offsets[i]));
	}

	vec3 col;
	for (int i = 0; i < 9; i++)
		col += sampleTex[i] * kernel[i];

	color = vec4(col, 1.0f);
	//float average = (color.r + color.g + color.b) / 3.0f;
	//color = vec4(average, average, average, 1.0f);
	//color = texture(myTexture, texCoords);
}
