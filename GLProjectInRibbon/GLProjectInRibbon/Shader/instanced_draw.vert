#version 400 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex_coords;
layout (location = 3) in vec3 barycentric;
layout (location = 4) in vec3 model_translate;

out vec3 Normal;
out vec3 FragPos;
out vec3 vBC;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(void)
{
	vec3 world_pos = (model * vec4(position, 1.0f)).xyz + model_translate;
	gl_Position = projection * view * vec4(world_pos, 1.0f);
	Normal = mat3(transpose(inverse(model))) * normal;
	FragPos = world_pos;
	vBC = barycentric;
}