#version 420 core
layout (depth_greater) out float gl_FragDepth;
out vec4 color;

void main()
{
	/*if(gl_FragCoord.x < 400)
		color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	else
		color = vec4(0.0f, 1.0f, 0.0f, 1.0f);*/
	color = vec4(1.0f);
	//gl_FragDepth = gl_FragCoord.z + 0.1f;
}