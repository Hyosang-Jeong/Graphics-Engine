#version 430 core
out vec4 outColor;
  
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gDiffuse;
uniform sampler2D gSpecular;
uniform sampler2D gShadowMap;


uniform vec3 eyePosition;   
uniform vec3 lightPosition; 

uniform mat4 lightSpaceMatrix;
uniform float near;
uniform float far;
out vec4 FragColor;



/*float ShadowCalculation(vec4 shadowCoord) 
{
    vec2 shadowIndex = shadowCoord.xy / shadowCoord.w;
    shadowIndex = shadowIndex * 0.5 + 0.5;
    float light_depth = texture(gShadowMap, shadowIndex).x; 
    float pixel_depth = (shadowCoord.w-near)/(far-near);
    float shadow = pixel_depth -0.05 > light_depth  ? 1.0 : 0.0;
    return shadow;
}*/

vec3 Cholesky_Decomposition(float m11, float m12, float m13, float m22, float m23, float m33, float z1, float z2, float z3)
{
    float a = sqrt(m11);
    float b = m12/a;
    float c = m13/a;
    float d = sqrt(m22 - (b*b));
    float e = (m23 - b*c)/d;
    float f = sqrt(m33 - c*c - e*e);
    float c1_ = z1/a;
    float c2_ = (z2 - b*c1_)/d;
    float c3_ = (z3 - c*c1_ - e*c2_)/f;
    vec3 result = vec3(0);
    result.z = c3_/f;
    result.y = (c2_ - e*result.z)/d;
    result.x = (c1_ - b*result.y - c*result.z)/a; 
    return result;
}


float MSM_Shadow(vec4 shadowCoord)
{
    vec2 shadowIndex = shadowCoord.xy / shadowCoord.w;
    shadowIndex = shadowIndex * 0.5 + 0.5;
    float zf = (shadowCoord.w-near)/(far-near);
    vec4 b = texture(gShadowMap, shadowIndex); 
    float a = 0.001;

    vec4 b_ = (1-a)*b + a * vec4(0.5);

    vec3 c = Cholesky_Decomposition(1,b_.x,b_.y,b_.y,b_.z,b_.w,1,zf,zf*zf);

    float z2,z3;
    if(c.y*c.y - 4*c.z*c.x < 0)
    {
        z2 = (-c.y)/(2*c.z);
        z3 = z2;
    }
    else
    {
        z2 = (-c.y-sqrt(c.y*c.y - 4*c.z*c.x))/(2*c.z);
        z3 = (-c.y+sqrt(c.y*c.y - 4*c.z*c.x))/(2*c.z);
    }


    if(zf <= z2)
        return 0;
    else if(zf <= z3)
    {
        return ((zf*z3) - (b_.x * (zf + z3)) + b_.y)/((z3-z2) * (zf-z2));
    }
    else
    {
        return 1 - (z2*z3 - b_.x*(z2+z3) + b_.y)/((zf-z2)*(zf-z3));
    }
}



void main()
{
    vec2 screenCoords = gl_FragCoord.xy / vec2(textureSize(gPosition, 0));

    vec3 worldPosition  = texture(gPosition, screenCoords).xyz;
    vec3 normal         = texture(gNormal, screenCoords).xyz;
    vec3 diffuseColor   = texture(gDiffuse, screenCoords).xyz;
    vec3 SpecularColor  = texture(gSpecular, screenCoords).xyz;
    vec3 ShadowMap      = texture(gShadowMap, screenCoords).xyz;

    vec3 lightVector    = normalize(lightPosition - worldPosition);
    vec3 viewVector     = normalize(eyePosition - worldPosition);
    vec3 reflectDir     = reflect(-lightVector, normal);

    float diffuseIntensity = max(dot(normal, lightVector), 0.0);
    vec3 diffuse =  diffuseColor * diffuseIntensity;

    float specularStrength = 0.5;
    float spec = pow(max(dot(viewVector, reflectDir), 0.0), 32.0);
    vec3 specular = specularStrength * spec * SpecularColor;

    vec4 shadowCoord = lightSpaceMatrix * vec4(worldPosition,1.0);
    float shadow = MSM_Shadow(shadowCoord);
    FragColor = (1.0 - shadow) *vec4(diffuse + specular, 1.0);
}