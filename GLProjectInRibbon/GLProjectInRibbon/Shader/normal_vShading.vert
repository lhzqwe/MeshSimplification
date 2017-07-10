#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec3 VColor;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform Material material;
uniform	DirLight dirLight;
uniform vec3 viewPos;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);
	vec3 WorldPos = vec3(model * vec4(position, 1.0f));
	vec3 WorldNormal = normalize(mat3(transpose(inverse(model))) * normal);
	vec3 viewDir = normalize(viewPos - WorldPos);
	VColor = CalcDirLight(dirLight, WorldNormal, viewDir);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	float diff = dot(normal, lightDir) * 0.5f + 0.5f;
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 ambient = light.ambient * material.ambient;
	vec3 diffuse = light.diffuse * diff * material.diffuse;
	vec3 specular = light.specular * spec * material.specular;
	return (ambient	+ diffuse + specular);
}