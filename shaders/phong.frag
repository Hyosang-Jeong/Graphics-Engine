#version 450 core
out vec4 outColor;


in vec3 Normal;  
in vec3 FragPos;  

uniform vec3 lightPos;


void main(void)
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);  
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 result =diff * vec3(1,1,1);
    outColor =  vec4(result,1);
}