#version 330 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gDiffuse;
layout (location = 3) out vec4 gSpecular;


in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform bool useDiffuseTexture;
uniform sampler2D diffuseTexture;
uniform vec3 diffuseValue;

uniform bool useSpecularTexture;
uniform sampler2D SpecularTexture;
uniform vec3 SpecularValue;
uniform float roughness;
uniform float metalness;

void main(void)
{
    gPosition = FragPos;
    gNormal = normalize(Normal);

    if(useDiffuseTexture)
        gDiffuse = texture(diffuseTexture, TexCoords).rgba;
    else
        gDiffuse = vec4(diffuseValue,roughness);

    if(useSpecularTexture)
        gSpecular = texture(SpecularTexture, TexCoords).rgba;
    else
        gSpecular = vec4(SpecularValue,metalness);

}