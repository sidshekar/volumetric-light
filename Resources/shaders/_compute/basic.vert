#version 440 core
layout (location = 0) in vec4 position;

//uniform mat4 vp;
out float outVSLifetime;

void main(void){
	
	gl_Position = vec4(position.xyz, 1.0);
	outVSLifetime = position.w;
}