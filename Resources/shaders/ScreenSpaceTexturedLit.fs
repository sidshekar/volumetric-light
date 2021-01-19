    #version 450 core



    in vec2 TexCoord;


    uniform vec3 cameraPos;
    uniform vec3 lightPos;
    uniform vec3 lightColor;

    uniform mat4 invViewMatrix;
    uniform mat4 invProjectionMatrix;

    uniform float ambientStrength;

    uniform sampler2D gPosition;
    uniform sampler2D gNormal;
    uniform sampler2D gAlbedoSpec;
    uniform sampler2D gDepth;

    uniform sampler2D shadowMapTexture;


    in vec2 farPlanePos;

    uniform float zFar;
    uniform float zNear;


    float g = 0.0f;




    uniform mat4 lightViewMatrix;
    uniform mat4 projectionMatrix;

    uniform mat4 viewMatrix;


    vec3 yellow_light = vec3(1,198.0/255.0,107.0/255.0);

    out vec4 finalColor;

    // use linear z depth
    vec3 ComputeWorldPos(float depth){

    vec4 pos = vec4(vec3(farPlanePos.x, farPlanePos.y, -zFar) * depth , 1.0f);
    vec4 ret = invViewMatrix * pos;
    return ret.xyz / ret.w;    
    }


    bool IsInShadow(vec4 worldPos){

    float fShadow = 0.0;
    vec4 lightPos = (lightViewMatrix * (worldPos));
    float fDistance = -lightPos.z / zFar;
    
    lightPos = projectionMatrix * lightPos;
 
    vec2 uv = lightPos.xy / lightPos.w * 0.5 + vec2(0.5f, 0.5f);
 
    uv.x = clamp(uv.x, 0.0f, 1.0f);
    uv.y = clamp(uv.y, 0.0f, 1.0f);
 
    float offset = 0.5f/zFar;
 
    float distanceMap = texture2D(shadowMapTexture, uv).r;
    return fDistance - offset > distanceMap;
    }

    void main(void){

     float depth = texture2D(gDepth, TexCoord).w;

     vec3 total_light;

    // volume light
    {
        float I = 0.0f;
        float d = depth * zFar;
        int virtual_plane_num = 100;
        int begin = int(virtual_plane_num * zNear / (d - zNear));
        int end = int(virtual_plane_num * (zFar - d) / (d - zNear));
        for(int j = begin; j <= virtual_plane_num + begin; j++)
        {
            float z = 1.0f * j / (begin + virtual_plane_num + end);

            vec3 pos = ComputeWorldPos(z);

            if(z < depth && !IsInShadow(vec4(pos,1.0f)))
            {
                //vec3 lightDis = pos - lightPos;
                //vec3 viewDis = pos - cameraPos;

                //float lightDis2 = lightDis.x * lightDis.x + lightDis.y * lightDis.y  + lightDis.z * lightDis.z;

                vec3 lightDir = normalize(pos - lightPos);
                vec3 viewDir = normalize(pos - cameraPos);

                float cosTheta = dot(lightDir,normalize(-lightPos));

                float hg = 1.0f/(4.0f*3.14f)* (1.0f - g*g)/ pow(1.0f + g * g - 2.0f * g * dot(lightDir,-viewDir), 1.5f);
                
                if(cosTheta >0.9){
                    I += clamp(10 * hg / virtual_plane_num, 0.0f, 1.0f);
                }
            }        
        }

        I = clamp(I , 0.0f,1.0f);
        total_light += I * yellow_light;

    }


        vec3 normal =   texture2D(gNormal, TexCoord).xyz * 2 - 1; //result.xyz * 2 - 1;
        vec3 worldPos = ComputeWorldPos(depth);

    
    // parallel lights
    /*
    {
        vec3 ViewDir = normalize( cameraPos - worldPos );
        vec3 lightDir = normalize(vec3(0.5,1,0.2) );
        vec3 halfDir = normalize(lightDir + ViewDir);
        float diffuse = 0.3 * clamp(dot(normal, lightDir), 0, 1) ;
        vec3 reflectDir = normalize(reflect(-lightDir,normal));
        float specular =  0.3 * pow(clamp(dot(reflectDir,halfDir),0,1),50.0);
        vec3 color = (diffuse + specular) *vec3(1,1,1);
        total_light += color;
    }
    */
    
    vec3 color = vec3(texture2D(gAlbedoSpec,TexCoord));
    float ambient = 0.1;
    
    finalColor = vec4(total_light + ambient * color,1);
    }



/*
void main(){
			
        vec3 Diffuse = texture(gAlbedoSpec, TexCoord).rgb;
		float spec = texture(gAlbedoSpec, TexCoord).a;

		// SSR
		vec3 albedo = texture(gAlbedoSpec, TexCoord).rgb;

		vec3 viewNormal = vec3(texture2D(gNormal, TexCoord));
		vec3 viewPos = textureLod(gPosition, TexCoord, 2).xyz;
		
		vec4 WorldPos4 = invViewMatrix * vec4 (viewPos, 1.0) ;
		vec4 Normal4 = invViewMatrix * vec4(viewNormal , 0.0);
		
		vec3 WorldPos = WorldPos4.xyz;
		vec3 Normal = Normal4.xyz; 
		
			
		// Lighting
		
		
		//**ambient
		vec3 ambient = ambientStrength * Diffuse.rgb;
		
		//**diffuse
		vec3 lightDir = normalize(lightPos - WorldPos);
		float diff = max(dot(Normal, lightDir), 0.0);
		vec3 diffuse = diff * lightColor;
		
		//**specular 
		vec3 viewDir = normalize(cameraPos - WorldPos);
		
		//** blinn Phong Model
		vec3 H = normalize( lightDir + viewDir);
		vec3 specular = pow(max(dot(Normal, H), 0.0), 128) * lightColor * spec;

		//** rim lighting
		float f = 1.0 - dot(Normal, viewDir);// Calculate the rim factor 
		f = smoothstep(0.0, 1.0, f);// Constrain it to the range 0 to 1 using a smoothstep function     
		f = pow(f, 16);// Raise it to the rim exponent 
		vec3 rim =  f * vec3(1.0f, 0.0f, 0.0f) * lightColor;// Finally, multiply it by the rim color

		// old lighting calculation
		vec3 totalColor = (ambient + diffuse + specular) * Diffuse.rgb; // * objectColor;

		//float shadow = ShadowCalculation(FragPosLightSpace);
		// new lighting calculation with shadow
		//vec3 totalColor = ambient + ((shadow) * (diffuse + specular + rim));
		
		vec3 lighting = totalColor;
		
		
		finalColor = vec4(lighting, 1.0f);
	
    }
		
        */

	
