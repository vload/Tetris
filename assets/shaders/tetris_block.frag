#version 430 core
out vec4 frag_color;

flat in int type;
in vec2 tex_coords;

uniform int level;

uniform sampler2D block_texture;
uniform sampler2D level_color_texture;

vec4 get_color(float level, float color_type){
    return texture(level_color_texture, vec2((level * 2.0 + color_type) / 32.0, 0.0), 0);
}

// use the level_color_texture to determine the color of the block and 
//     the block_texture to determine which color to use
void main()
{
    float r = texture(block_texture, tex_coords).r;
    if(r < 0.1){
        frag_color = vec4(0.0, 0.0, 0.0, 1.0);
    }
    else if(r < 0.5){
        frag_color = get_color(level, 0);
    }
    else if(r < 0.9){
        frag_color = get_color(level, 1);
    }
    else{
        frag_color = vec4(1.0, 1.0, 1.0, 1.0);
    }
}
