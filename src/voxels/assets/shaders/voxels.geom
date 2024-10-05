#version 430 core

layout (points) in;
layout (triangle_strip, max_vertices = 18) out;

in vec3 vert_color[];
out vec3 color;

uniform mat4 view;
uniform mat4 proj;
uniform float voxel_size;

// generate a voxel from a single point with the given proj and view matrices
void main()
{
    // set the color
    color = vert_color[0];

    // Get the input point position
    vec3 p = gl_in[0].gl_Position.xyz; 
    
    // Define the 8 corners of the voxel (a cube centered around the input point `p`)
    float half_edge = voxel_size / 2;               // Half of the voxel's edge size
    vec4 v0 = proj * view * vec4(p + vec3(-half_edge,  half_edge, -half_edge), 1.0);
    vec4 v1 = proj * view * vec4(p + vec3(-half_edge, -half_edge, -half_edge), 1.0);
    vec4 v2 = proj * view * vec4(p + vec3( half_edge,  half_edge, -half_edge), 1.0);
    vec4 v3 = proj * view * vec4(p + vec3( half_edge, -half_edge, -half_edge), 1.0);
    vec4 v4 = proj * view * vec4(p + vec3( half_edge,  half_edge,  half_edge), 1.0);
    vec4 v5 = proj * view * vec4(p + vec3( half_edge, -half_edge,  half_edge), 1.0);
    vec4 v6 = proj * view * vec4(p + vec3(-half_edge,  half_edge,  half_edge), 1.0);
    vec4 v7 = proj * view * vec4(p + vec3(-half_edge, -half_edge,  half_edge), 1.0);

    // // side faces
    gl_Position = v0; 
    EmitVertex();
    gl_Position = v1; 
    EmitVertex();
    gl_Position = v2; 
    EmitVertex();
    gl_Position = v3; 
    EmitVertex();
    gl_Position = v4; 
    EmitVertex();
    gl_Position = v5; 
    EmitVertex();
    gl_Position = v6; 
    EmitVertex();
    gl_Position = v7; 
    EmitVertex();
    gl_Position = v0; 
    EmitVertex();
    gl_Position = v1; 
    EmitVertex();
    EndPrimitive();

    // top face
    gl_Position = v0; 
    EmitVertex();
    gl_Position = v2; 
    EmitVertex();
    gl_Position = v6; 
    EmitVertex();
    gl_Position = v4;
    EmitVertex();
    EndPrimitive();

    // bottom face
    gl_Position = v1; 
    EmitVertex();
    gl_Position = v3; 
    EmitVertex();
    gl_Position = v7; 
    EmitVertex();
    gl_Position = v5; 
    EmitVertex();
    EndPrimitive();
} 