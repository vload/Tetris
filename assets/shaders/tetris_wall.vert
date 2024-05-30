#version 430 core
layout (location = 0) in vec2 in_position;
layout (location = 1) in float in_color;

uniform mat4 projection;

out vec4 vert_color;

void main()
{
    gl_Position = projection * vec4(in_position, 0.0, 1.0);
    vert_color = vec4(vec3(in_color), 1.0);
}