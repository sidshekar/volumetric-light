#version 450 core

out vec4 fragColor;

in vec2 TexCoord;

uniform mat4 projMatrix;
uniform mat4 invViewMatrix;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D gLighting;

const float step = 0.05;
const float minRayStep = 0.1;
const float maxSteps = 50;
const int numBinarySearchSteps = 5;
const float reflectionSpecularFalloffExponent = 3.0;
const float strength = 0.3;

#define Scale vec3(.8, .8, .8)
#define K 19.19

vec3 BinarySearch(inout vec3 dir, inout vec3 hitCoord, inout float dDepth);
vec4 RayMarch(vec3 dir, inout vec3 hitCoord, out float dDepth);

vec3 fresnelSchlick(float cosTheta, vec3 F0){
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 hash(vec3 a){
    a = fract(a * Scale);
    a += dot(a, a.yxz + K);
    return fract((a.xxy + a.yxx)*a.zyx);
}


vec3 BinarySearch(inout vec3 dir, inout vec3 hitCoord, inout float dDepth)
{
    float depth;

    vec4 projectedCoord;
 
    for(int i = 0; i < numBinarySearchSteps; i++)
    {

        projectedCoord = projMatrix * vec4(hitCoord, 1.0);
        projectedCoord.xy /= projectedCoord.w;
        projectedCoord.xy = projectedCoord.xy * 0.5 + 0.5;
 
        depth = textureLod(gPosition, projectedCoord.xy, 2).z;

 
        dDepth = hitCoord.z - depth;

        dir *= 0.5;
        if(dDepth > 0.0)
            hitCoord += dir;
        else
            hitCoord -= dir;    
    }

        projectedCoord = projMatrix * vec4(hitCoord, 1.0);
        projectedCoord.xy /= projectedCoord.w;
        projectedCoord.xy = projectedCoord.xy * 0.5 + 0.5;
 
    return vec3(projectedCoord.xy, depth);
}

vec4 RayMarch(vec3 dir, inout vec3 hitCoord, out float dDepth)
{
    dir *= step;
 
    float depth;
    int steps;
    vec4 projectedCoord;

 
    for(int i = 0; i < maxSteps; i++)
    {
        hitCoord += dir;
 
        projectedCoord = projMatrix * vec4(hitCoord, 1.0);
        projectedCoord.xy /= projectedCoord.w;
        projectedCoord.xy = projectedCoord.xy * 0.5 + 0.5;
 
        depth = textureLod(gPosition, projectedCoord.xy, 2).z;
        
		
		if(depth > 1000.0)
            continue;
 
        dDepth = hitCoord.z - depth;

        if((dir.z - dDepth) < 1.2)
        {
            if(dDepth <= 0.0)
            {   
                vec4 Result;
                Result = vec4(BinarySearch(dir, hitCoord, dDepth), 1.0);

                return Result;
            }
        }
        
        steps++;
    }
 
    
    return vec4(projectedCoord.xy, depth, 0.0);
}



void main(){
		
		vec2 tCoord = vec2 (TexCoord.x, 1 - TexCoord.y);
		vec3 Diffuse = texture(gAlbedoSpec, tCoord).rgb;
		float spec = texture(gAlbedoSpec, tCoord).a;

		// SSR
		
		vec3 viewNormal = vec3(texture2D(gNormal, tCoord));
		vec3 viewPos = textureLod(gPosition, tCoord, 2).xyz;
		
		vec3 albedo = texture(gLighting, tCoord).rgb;
		vec3 lighting = texture(gLighting, tCoord).rgb;
		vec3 ssr;
		vec3 Fresnel;
		
		if(spec > 0.0){
        
		
		
			vec3 F0 = vec3(0.04); 
			F0      = mix(F0, albedo, spec);
			
			Fresnel = fresnelSchlick(max(dot(normalize(viewNormal), normalize(viewPos)), 0.0), F0);
		
			// Reflection vector
			vec3 reflected = normalize(reflect(normalize(viewPos), normalize(viewNormal)));
		
			vec3 hitPos = viewPos;
			float dDepth;
			
			vec3 worldpos = vec3(vec4(viewPos, 1.0) * invViewMatrix);
			vec3 jitt = mix(vec3(0.0), vec3(hash(worldpos)), 0.5f); 
			
			//-- Enable jitter
			vec4 coords = RayMarch((vec3(jitt) + reflected * max(minRayStep, -viewPos.z)), hitPos, dDepth);
	
			//-- Disable jittering
			//vec4 coords = RayMarch((reflected * max(minRayStep, -viewPos.z)), hitPos, dDepth);
			
			vec2 dCoords = smoothstep(0.2, 0.6, abs(vec2(0.5, 0.5) - coords.xy));
			
			float screenEdgefactor = clamp(1.0 - (dCoords.x + dCoords.y), 0.0, 1.0);
			
			   float ReflectionMultiplier = pow(spec, reflectionSpecularFalloffExponent) * 
                screenEdgefactor * 
                -reflected.z;
		
			ssr = textureLod(gLighting, coords.xy, 2).rgb * clamp(ReflectionMultiplier, 0.0, 0.9)* Fresnel ;
			
		}
		
		fragColor = vec4((ssr + lighting) , 1.0f) ;		
    }
		

		
