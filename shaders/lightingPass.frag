#version 430 core
out vec4 outColor;
  
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gDiffuse;
uniform sampler2D gSpecular;
uniform sampler2D gShadowMap;
uniform sampler2D sky;
uniform sampler2D sky_irr;

uniform vec3 eyePosition;   
uniform vec3 lightPosition; 
uniform vec3 lightColor;
uniform mat4 lightSpaceMatrix;
uniform float near;
uniform float far;

uniform float N;
uniform float exposure;


uniform HammersleyBlock {
 float hammersley[2*100]; 
}block;



out vec4 FragColor;

float PI = 3.14159;
const vec2 invAtan = vec2(0.1591, 0.3183);
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



float D(vec3 H, vec3 N, float roughness)
{
   float a = (roughness + 2.0)/(2.0*PI);
   a*=pow(dot(N,H),roughness);
   return a;
}

vec3 F(vec3 L, vec3 H, vec3 Ks)
{
    float cosTheta = max(dot(L,H),0.0);
    return Ks + (1.0 - Ks) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float G1(vec3 V, vec3 H, vec3 N, float a)
{
    float vN = max(dot(V,N),0.0);
    if(vN > 1.0)
        return 1.0;
    float tan_theta = sqrt(1.0 - vN*vN)/vN;

    if(tan_theta == 0)
        return 1.0;

    float alpha = sqrt(a*0.5 + 1)/tan_theta;

    if(alpha < 1.6)
    {
        return (3.535*a + 2.181*a*a)/(1.0+2.276*a+2.577*a*a);
    }

    return 1;
}

float G(vec3 L, vec3 V, vec3 H, vec3 N, float roughness)
{
    return G1(L,H,N, roughness)*G1(V,H,N, roughness);
}

vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}



void main()
{
    vec2 screenCoords = gl_FragCoord.xy / vec2(textureSize(gPosition, 0));


    vec3 worldPosition  = texture(gPosition, screenCoords).xyz;
    vec3 normal         = texture(gNormal, screenCoords).xyz;
    vec4 diffuseColor   = texture(gDiffuse, screenCoords).xyzw;
    vec4 SpecularColor  = texture(gSpecular, screenCoords).xyzw;
    vec3 ShadowMap      = texture(gShadowMap, screenCoords).xyz;
    float roughness     = diffuseColor.w;
    float metalness     = SpecularColor.w;
    vec3 lightVector    = normalize(lightPosition - worldPosition);
    vec3 viewVector     = normalize(eyePosition - worldPosition);

    vec4 shadowCoord = lightSpaceMatrix * vec4(worldPosition,1.0);
    float shadow = MSM_Shadow(shadowCoord);

    SpecularColor.xyz = mix(SpecularColor.xyz,diffuseColor.xyz,metalness);

    vec3 spec = vec3(0);


    vec3 R = normalize(2*dot(viewVector,normal)*normal - viewVector);
    vec3 H = normalize(lightVector+viewVector);
    vec3 A = normalize(cross(vec3(0,0,1), R));
    vec3 B = normalize(cross(R,A));

    float cosTheta = max(dot(normal,lightVector),0);


    for(int i = 0; i < N*2; i+=2)
    {
        float ran1 = block.hammersley[i];
        float ran2 = block.hammersley[i+1];

        float theta = acos(pow(ran2,1.0/(roughness+1.0)));

        float u = ran1;
        float v = theta/PI;


        vec3 L = vec3(cos(2*PI*(0.5-u))*sin(PI*v), sin(2*PI*(0.5-u))*sin(PI*v),cos(PI*v));

        vec3 w = normalize(L.x*A + L.y*B + L.z*R);

        //vec2 uv = vec2(0.5-atan(-w.z, -w.x)/(2*PI), acos(-w.y)/PI);

        vec2 uv = SampleSphericalMap(w);
        vec3 h = normalize(w+viewVector);

        float level = 0.5*log2(100*100/N) - 0.5*log2(D(h,normal,roughness));

        vec3 color = textureLod(sky,uv,level).xyz;

        
        spec += 
        dot(w,normal) * color* G(w,viewVector,h,normal,roughness)*F(w,h,SpecularColor.xyz)/
        (4.0*max(dot(w, normal),0.1)*dot(viewVector,normal)+ 0.0001);
    }
    spec /= N;
    vec2 uv = SampleSphericalMap(normal);
    vec3 Diffuse = (diffuseColor.xyz/PI) * texture2D(sky_irr,uv).xyz ;

    vec3 IBL = Diffuse + spec;

    vec3 BRDF = lightColor*cosTheta*diffuseColor.xyz*(1-shadow)/PI  + 
    (D(H,normal,roughness)*G(lightVector,viewVector,H,normal,roughness)*F(lightVector,H,SpecularColor.xyz))
    /(4.0*dot(lightVector,normal)*dot(viewVector,normal) +0.0001);
    
    BRDF = clamp(lightColor*cosTheta*BRDF,0.0,1.0);



    vec3 color = BRDF + IBL;
    color = (exposure*color) / (exposure*color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2)); 

    FragColor = vec4(color,1.0);
}