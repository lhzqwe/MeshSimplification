#version 410 core

uniform vec2 lod_distance;

layout(points) in;
layout(points, max_vertices = 1) out;

in vec3 VS_view_position[1];
in vec3 VS_model_translate[1];

layout(stream=0) out vec3 model_translate_lod0;
layout(stream=1) out vec3 model_translate_lod1;
layout(stream=2) out vec3 model_translate_lod2;

void main(void)
{
	float distance = length(VS_view_position[0]);
	if(distance < lod_distance.x)
	{
		model_translate_lod0 = VS_model_translate[0];
		EmitStreamVertex(0);
	}
	else if(distance < lod_distance.y)
	{
		model_translate_lod1 = VS_model_translate[0];
		EmitStreamVertex(1);
	}
	else
	{
		model_translate_lod2 = VS_model_translate[0];
		EmitStreamVertex(2);
	}
}