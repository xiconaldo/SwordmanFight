#version 430 core

layout (location = 3) uniform vec3 light = vec3(0.0f, 0.0f, -1.0f);

layout (binding  = 0) uniform sampler2D grass;
layout (binding  = 1) uniform sampler2D height_map;

in 	vec2 text_coord;
out vec4 color;

void main(){
	vec3 normal = texture(height_map, text_coord).rgb;

	vec3 local_light = normalize(light);
	normal = normalize(normal);

	float intensity = -dot(normal, local_light);
	if(intensity < 0.4f) intensity = 0.4f;	

	vec4 text_color = texture(grass, text_coord*50.0f);
	color = vec4(text_color.xyz * pow(intensity, 2), text_color.a);
}