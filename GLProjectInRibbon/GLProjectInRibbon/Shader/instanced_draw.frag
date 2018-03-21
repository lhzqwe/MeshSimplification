#version 400 core

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

in vec3 Normal;
in vec3 FragPos;
in vec3 vBC;

out vec4 color;

uniform vec3 viewPos;
uniform	DirLight dirLight;
uniform Material material;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
float edgeFactor();

void main()
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 result = CalcDirLight(dirLight, norm, viewDir);
	color = vec4(mix(vec3(0.0f, 0.0f, 0.0f), result, edgeFactor()), 1.0f);
	//color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	//if(any(lessThan(vBC, vec3(0.005f))))
	//{
		//result = vec3(0.0f);
	//}
	//color = vec4(result, 1.0f);
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

float edgeFactor()
{
	vec3 d = fwidth(vBC);
	vec3 a3 = smoothstep(vec3(0.0), d*1.5, vBC);
	return min(min(a3.x, a3.y), a3.z);
}
