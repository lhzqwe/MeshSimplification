#version 330 core

in vec3 VColor;

out vec4 color;

void main()
{	
	color = vec4(VColor, 1.0f);
}


