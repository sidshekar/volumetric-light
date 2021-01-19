#version 450 core

out vec4 color;

uniform sampler2D Texture;

in vec2 TexCoord;

void main(){
		
	vec2 tCoord = vec2 (TexCoord.x, 1- TexCoord.y);
		
	float outcolor = texture(Texture, tCoord).r;
	
	color = vec4(pow(outcolor, 24.0f));
	
	
	
}