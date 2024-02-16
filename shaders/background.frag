#version 450 core
out vec4 outColor;
uniform sampler2D diffuseTexture;
in vec2 TexCoords;
void main(void)
{
    vec3 color = texture(diffuseTexture, TexCoords).rgb;
    outColor =  vec4(color,1);
}