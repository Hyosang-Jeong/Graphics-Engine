#version 450 core
out vec4 outColor;

layout (location = 0) out vec4 depthbuffer;

in vec4 position;
out vec4 fragColor;

uniform float near;
uniform float far;
void main(void)
{
    float depth = (position.w-near)/(far-near);
    depthbuffer = vec4(depth, depth*depth, depth*depth*depth, depth*depth*depth*depth);
}