#version 450 core
out vec4 outColor;

uniform sampler2D diffuseTexture;

in vec3 fragPos;
const vec2 invAtan = vec2(0.1591, 0.3183);
uniform float exposure;
vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main()
{       
    vec2 uv = SampleSphericalMap(normalize(fragPos));

    vec3 color = texture(diffuseTexture, uv).rgb;
    
    color = (exposure*color) / (exposure*color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2)); 

    outColor = vec4(color, 1.0);
}
