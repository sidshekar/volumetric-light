#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 lightViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 model;

out vec2 depth;

void main()
{
   //gl_Position = projectionMatrix * lightViewMatrix * model * vec4(position, 1.0);


   gl_Position = model * vec4(position, 1.0f);
   gl_Position = lightViewMatrix * gl_Position;
    
   depth = gl_Position.zw;
    
   gl_Position = projectionMatrix * gl_Position;
}  