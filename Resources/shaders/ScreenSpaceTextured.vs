#version 450 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

uniform mat4 model;

out vec2 TexCoord;

void main(void){
	
	gl_Position =  model * vec4(position, 1.0);
	TexCoord = texCoord;
	
}