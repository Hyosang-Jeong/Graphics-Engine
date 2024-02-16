#version 430 core
out vec4 outColor;
  
uniform sampler2D debugTexture;

out vec4 FragColor;


void main()
{
    vec2 screenCoords = gl_FragCoord.xy / vec2(1600,1000);

    FragColor = vec4(texture(debugTexture, screenCoords).xyz, 1.0);
}