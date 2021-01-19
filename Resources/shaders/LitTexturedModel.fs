#version 450 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec3 viewPos;
in vec2 TexCoord;
in vec3 viewNormal;

uniform float specularStrength;

out vec4 fragColor;

// texture
uniform sampler2D Texture;

void main(){
		
		gPosition = viewPos;
		
		gNormal = viewNormal;
		
		gAlbedoSpec.rgb = texture(Texture, TexCoord).rgb;
		
		gAlbedoSpec.a = specularStrength;
		
}