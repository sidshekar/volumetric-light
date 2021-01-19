#version 450 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

out vec3 viewPos;
out vec2 TexCoord;
out vec3 viewNormal;

uniform mat4 view;
uniform mat4 model;
uniform mat4 proj;

void main(){

	
	vec4 vpos = view * model * vec4(position, 1.0);
	viewPos = vpos.xyz;
	
	vec3 n = normalize((model * vec4(normal, 0.0)).xyz);
    viewNormal = normalize((view * vec4(n,0)).xyz);
	
	TexCoord = texCoord;
	
	gl_Position = proj * view * model *vec4(position, 1.0);
}