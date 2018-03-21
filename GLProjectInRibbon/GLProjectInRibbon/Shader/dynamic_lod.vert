#version 410 core

layout (location = 0) in vec3 model_translate;

uniform mat4 view_matrix;

out vec3 VS_view_position;
out vec3 VS_model_translate;

void main(void)
{
	gl_Position = view_matrix * vec4(model_translate, 1.0f);
	VS_view_position = gl_Position.xyz;
	VS_model_translate = model_translate;
}
