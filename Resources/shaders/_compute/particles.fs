#version 330 core

//out float opacity;
uniform sampler2D Texture;

out vec4 color;

in GS_FS_VERTEX{
	vec2 texcoord;
}fs_in;


void main(){
  

  
	color =  texture(Texture, vec2(fs_in.texcoord.x , fs_in.texcoord.y));//* vec4(123.0f/255.0f, 173.0f/255.0f, 203.0f/255.0f, 1.0f);
	
}  