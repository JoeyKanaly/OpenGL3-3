#version 330 core

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shine;
};

struct Light
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

out vec4 color;

void main()
{
	// Ambient
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

	// Diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

	// Specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shine);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

	// Attenuation
	float dist = length(light.position - FragPos);
	float attenuation = 1.0f / (light.constant + light.linear * dist + light.quadratic * pow(dist, 2.0f));
	
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	vec3 result = ambient + diffuse + specular;

	color = vec4(result, 1.0f);
}