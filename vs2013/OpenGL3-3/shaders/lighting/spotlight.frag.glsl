#version 330 core 

struct Light
{
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shine;	
};

uniform vec3 viewPos;
uniform Light light;
uniform Material material;

in vec3 fragPos;
in vec3 normal;
in vec2 TexCoords;

out vec4 color;

void main()
{
	vec3 lightDir = normalize(light.position - fragPos);
	// Ambient
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

	// Diffuse
	vec3 norm = normalize(normal);
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

	// Specular
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shine);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

	//Soft Edges
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);
	diffuse *= intensity;
	specular *= intensity;

	// Attenuation
	float dist = length(light.position - fragPos);
	float attenuation = 1.0f / (light.constant + light.linear * dist + light.quadratic * pow(dist, 2.0f));
	
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	vec3 result = ambient + diffuse + specular;
		
	color = vec4(result, 1.0f);
}