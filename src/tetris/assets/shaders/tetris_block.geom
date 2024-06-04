#version 430 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in int vert_type[];
out int type;
out vec2 tex_coords;

uniform mat4 projection;

// generate a square quad from a single point with the given projection matrix
void main()
{
    type = vert_type[0];

    gl_Position = gl_in[0].gl_Position + projection * vec4(0.0, 0.0, 0.0, 0.0);
    tex_coords = vec2(0.0, 0.0);
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + projection * vec4(1.0, 0.0, 0.0, 0.0);
    tex_coords = vec2(1.0, 0.0);
    EmitVertex();
    
    gl_Position = gl_in[0].gl_Position + projection * vec4(0.0, 1.0, 0.0, 0.0); 
    tex_coords = vec2(0.0, 1.0);
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + projection * vec4(1.0, 1.0, 0.0, 0.0);
    tex_coords = vec2(1.0, 1.0);
    EmitVertex();
    
    EndPrimitive();
} 