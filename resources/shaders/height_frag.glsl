#version 330 core
layout(location = 0) out vec4 color;
layout(location = 1) out vec4 colorBW;

in vec3 frag_tex;
in vec3 vertex_normal;

void main()
{
color = vec4(.2,1,.2,1);
color.rgb = frag_tex;
    colorBW = vec4(0, 0, 0, 1);
}
