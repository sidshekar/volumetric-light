#version 450 core

out vec4 color;

uniform sampler2D Texture;

in vec2 TexCoord;
uniform mat4 invView;

uniform float rtType;

void main(){
		
	vec2 tCoord = vec2 (TexCoord.x, 1- TexCoord.y);
		
	vec3 viewPosToPos3 =  texture(Texture, tCoord).xyz;
	
	if(rtType >= 0){
	
	vec4 viewPosToPos4 = invView * vec4 (viewPosToPos3, rtType) ;
	
	color = vec4(viewPosToPos4.xyz, 0.0f);
	
	}else{
	
	color = vec4(viewPosToPos3.xyz, 0.0f);
	
	}
	
}