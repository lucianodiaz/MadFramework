#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;   // color del vértice
layout(location = 2) in vec2 aTexCoord;

out vec4 vColor;   // pasamos el color al fragment
out vec2 vTexCoord;

void main()
{
    vColor = aColor;
    vTexCoord = aTexCoord;
    gl_Position = vec4(aPos, 1.0);
}