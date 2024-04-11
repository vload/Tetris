#version 430 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in vec4 vert_color[];
out vec4 color;

uniform mat4 projection;

void main()
{
    color = vert_color[0];

    gl_Position = gl_in[0].gl_Position + projection * vec4(0.0, 0.0, 0.0, 0.0);
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + projection * vec4(1.0, 0.0, 0.0, 0.0);
    EmitVertex();
    
    gl_Position = gl_in[0].gl_Position + projection * vec4(0.0, 1.0, 0.0, 0.0); 
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + projection * vec4(1.0, 1.0, 0.0, 0.0);
    EmitVertex();
    
    EndPrimitive();
} 