#version 330 core

out vec4 color;

uniform vec3 a2f_color;

void main()
{
	color = vec4(a2f_color, 1.0f);
}