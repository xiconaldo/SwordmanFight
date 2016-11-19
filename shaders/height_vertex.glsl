#version 430 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

layout (location = 0) uniform mat4 model = mat4(1.0f);
layout (location = 1) uniform mat4 view  = mat4(1.0f);
layout (location = 2) uniform mat4 proj  = mat4(1.0f);
layout (location = 3) uniform vec3 light = vec3(-1.0f, -1.0f, -1.0f);
layout (location = 4) uniform mat4 rev_model = mat4(1.0f);

layout (std140, binding = 0) uniform Player{
	float x;
	float z;
} player;

layout (binding  = 5) uniform sampler2D height_map;

out float intensity;
out vec2 text_coord;

void main(){
	vec4 pos = vec4(vertex, 1.0f);

	mat4 m_model = model;
	m_model[3].y += texture(height_map, model[3].xz * 0.5f + 0.5f).a * 0.15f;
	
	m_model[3].x -= player.x;
	m_model[3].z -= player.z;
	m_model[3].y -= texture(height_map, vec2(player.x, player.z) * 0.5f + 0.5f).a * 0.15f + 0.038f;

	vec3 local_light = normalize(light);
	//vec3 norm_coord = normalize(rev_model * vec4(normal, 1.0f)).xyz;
	vec3 norm_coord = normal;
	intensity = -dot(norm_coord, local_light);
	if(intensity < 0.4) intensity = 0.4;

	gl_Position = proj * view * m_model * pos;
	text_coord = uv;
}