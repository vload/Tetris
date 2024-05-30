#version 430 core

layout (location = 0) in vec2 in_position;
layout (location = 1) in int in_type;

out int vert_type;

uniform mat4 projection;

// pass through the position and type
void main()
{
    gl_Position = projection * vec4(in_position, 0.0, 1.0);
    vert_type = in_type;
}