#version 430 core
out vec4 frag_color;

flat in vec3 color;

void main()
{
    frag_color = vec4(color, 1.0);
}
