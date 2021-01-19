#version 440 core


out vec4 color;
in float outGSLifetime;

in GS_FS_VERTEX{
	vec2 texcoord;
}fs_in;

// texture
uniform sampler2D Texture;
uniform float currentTime;

float random (vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898,78.233)))* 43758.5453123);
}
void main(){
		
		
	color = vec4(0.0f,0.48f, 0.58f, outGSLifetime) * texture(Texture, vec2(fs_in.texcoord.x , fs_in.texcoord.y));
	
}