#version 430 core
out vec4 outColor;
  
in vec3 FragPos;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gDiffuse;
uniform sampler2D gSpecular;

uniform vec3 lightPosition; 
uniform float radius;
uniform vec3 diffuseValue;
uniform vec3 eyePosition;   
out vec4 FragColor;

void main()
{
    vec2 screenCoords   = gl_FragCoord.xy / vec2(textureSize(gPosition, 0));
    vec3 worldPosition  = texture(gPosition, screenCoords).xyz;
    vec3 normal         = texture(gNormal, screenCoords).xyz;
    vec3 diffuseColor   = texture(gDiffuse, screenCoords).xyz;
    vec3 SpecularColor  = texture(gSpecular, screenCoords).xyz;


    float Distance      = length(lightPosition - worldPosition);
    vec3 lightVector    = normalize(lightPosition - worldPosition);
    vec3 viewVector     = normalize(eyePosition - worldPosition);
    vec3 reflectDir     = reflect(-lightVector, normal);

    float diffuseIntensity  = max(dot(normal, lightVector), 0.0);
    vec3 diffuse =  diffuseColor * diffuseIntensity;
    float specularStrength  = 0.5;
    float spec              = pow(max(dot(viewVector, reflectDir), 0.0), 32.0);
    vec3 specular           = specularStrength * spec * SpecularColor;
    if (Distance > radius)
    {
        FragColor = vec4(0.0, 0.0, 0.0, 0.0);
    }
    else
    {
        // Calculate light contribution using lighting equation and attenuation
        float attenuation = (1.0 / (Distance * Distance)) - (1.0 / (radius * radius));
        FragColor =  attenuation* vec4(diffuse + specular,1.0) * vec4(diffuseValue, 0.0);
    }


}