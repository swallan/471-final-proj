#version 330 core
out vec4 color;

in vec3 vertex_pos;
in vec2 vertex_tex;
uniform vec3 campos;

uniform sampler2D tex;
uniform sampler2D tex2;
void main()
{
    color = texture(tex2, vertex_tex);
    color.a = pow(color.r,.5);
    //color.a = 1;
    

}
