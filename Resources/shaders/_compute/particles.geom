#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform mat4 vp;
uniform mat4 m;
uniform vec3 vQuad1, vQuad2;

out GS_FS_VERTEX{
	vec2 texcoord;
}gs_out;


in float outVSLifetime[];

out float outGSLifetime;

void buildQuad(float size, mat4 vp){
	

	outGSLifetime = outVSLifetime[0];
	
	vec3 p1 =  gl_in[0].gl_Position.xyz +(-vQuad1-vQuad2)* size; gl_Position = vp * vec4(p1, 1.0f); gs_out.texcoord = vec2(0.0f, 0.0f); EmitVertex(); // vertex 0
	vec3 p2 =  gl_in[0].gl_Position.xyz + (-vQuad1+vQuad2)* size;  gl_Position = vp *  vec4(p2, 1.0f); gs_out.texcoord = vec2(0.0f, 1.0f); EmitVertex(); // vertex 1
	vec3 p3 =  gl_in[0].gl_Position.xyz + (vQuad1-vQuad2)* size; gl_Position = vp * vec4(p3, 1.0f); gs_out.texcoord = vec2(1.0f, 0.0f); EmitVertex(); // vertex 2
	vec3 p5 =  gl_in[0].gl_Position.xyz + (vQuad1+vQuad2)* size;   gl_Position = vp * vec4(p5, 1.0f); gs_out.texcoord = vec2(1.0f, 1.0f); EmitVertex(); // vertex 3


	
	EndPrimitive();
	
	}

	void main() {    
	
		buildQuad(.50f, vp);	
	
	}