#version 330 core

in vec3 FragPos;
in vec3 normal;

out vec4 color;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform mat4 model2;
void main()
{
	float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength * lightColor;
	vec4 lightPos2 = model2 * vec4(lightPos, 1.0f);
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos2.xyz - FragPos);

	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = diff * lightColor;

	float specularStrength = 0.5f;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 1024);
	vec3 specular = specularStrength * spec * lightColor;

	vec3 result = (ambient + diffuse + specular) * objectColor;
	color = vec4(result, 1.0f);
}