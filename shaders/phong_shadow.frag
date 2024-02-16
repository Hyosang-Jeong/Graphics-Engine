#version 450 core
out vec4 outColor;


in vec3 Normal;  
in vec3 FragPos;  
in vec4 shadowCoord;
in vec2 texCoord;

uniform sampler2D shadowMap;
uniform vec3 lightPos;
 

float ShadowCalculation()
{
    vec2 shadowIndex = shadowCoord.xy / shadowCoord.w;
    // transform to [0,1] range
    shadowIndex = shadowIndex * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float light_depth = texture(shadowMap, shadowIndex).x; 
    // get depth of current fragment from light's perspective
    float pixel_depth = shadowCoord.w;
    // check whether current frag pos is in shadow
    float shadow = pixel_depth -0.05 > light_depth  ? 1.0 : 0.0;
    return shadow;
}


void main(void)
{
    vec3 ambient = vec3(1,1,1);
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);  
    float diff = max(dot(norm, lightDir), 0.0);

    float shadow = ShadowCalculation(); 

    vec3 result =((1.0 - shadow) * (diff)) * ambient;  
    outColor =  vec4(result,1);
}