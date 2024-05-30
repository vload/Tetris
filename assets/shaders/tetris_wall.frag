#version 430 core
out vec4 frag_color;

in vec4 vert_color;

// Draw white walls
void main()
{
    frag_color = vert_color;
}
