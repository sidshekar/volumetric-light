#version 450 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

uniform mat4 model;
uniform mat4 vp;

uniform float zFar;
uniform float fov;;
uniform float aspectRatio;

out vec2 TexCoord;
out vec2 farPlanePos;

void main(void){
	
	gl_Position =  vec4(position, 1.0);
	
	TexCoord = vec2 (texCoord.x, 1 - texCoord.y);

	float t = tan(fov/2);
	farPlanePos.x = (TexCoord.x * 2 - 1) * zFar * t * aspectRatio;
    farPlanePos.y = (TexCoord.y * 2 - 1) * zFar * t;
	
}