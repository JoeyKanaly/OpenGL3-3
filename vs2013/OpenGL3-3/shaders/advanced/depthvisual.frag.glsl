#version 330 core

out vec4 color;

float linearizeDepth(float depth)
{
	float near = 0.1f;
	float far = 100.0f;
	float z = depth * 2.0f - 1.0f;
	return (2.0f * near) / (far + near - z * (far - near));
}

void main()
{
	color = vec4(vec3(linearizeDepth(gl_FragCoord.z)), 1.0f);
}