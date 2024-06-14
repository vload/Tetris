#version 430 core

layout (location = 0) in vec2 in_position;
layout (location = 1) in vec3 in_color;

out vec3 vert_color;

void main()
{
    gl_Position = vec4(in_position, 0.0, 1.0);
    vert_color = in_color;
}