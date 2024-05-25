#version 430 core
out vec4 frag_color;

in vec4 color;
in vec2 tex_coords;

uniform sampler2D block_texture;

void main()
{
    frag_color = texture(block_texture, tex_coords) * color;  
}
