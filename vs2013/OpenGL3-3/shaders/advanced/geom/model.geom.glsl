#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT
{
	vec2 texture;
} gs_in[];

uniform float time;

out vec2 fTexture;

vec3 GetNormal()
{
	vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
	vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
	return normalize(cross(a, b));
}

vec4 explode(vec4 position, vec3 normal)
{
	float magnitude = 2.0f;
	vec3 direction = normal * ((sin(time) +1.0f) / 2.0f) * magnitude;
	return position + vec4(direction, 0.0f);
}

void main()
{
	vec3 normal = GetNormal();
	gl_Position = explode(gl_in[0].gl_Position, normal);
	fTexture = gs_in[0].texture;
	EmitVertex();
	gl_Position = explode(gl_in[1].gl_Position, normal);
	fTexture = gs_in[1].texture;
	EmitVertex();
	gl_Position = explode(gl_in[2].gl_Position, normal);
	fTexture = gs_in[2].texture;
	EmitVertex();
	EndPrimitive();
}