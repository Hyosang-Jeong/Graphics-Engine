#version 450 core
layout (location = 0) in vec3 aPos;


uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec4 position;
void main()
{
    gl_Position = projection * view  *model* vec4(aPos, 1.0);
    position = gl_Position;
}