#version 330 core
layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec2 vertTex;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
out vec3 vertex_pos;
out vec2 vertex_tex;


uniform sampler2D tex;
uniform sampler2D tex2;

void main()
{
    float height = texture(tex, vertTex).r;
    height *=20.0;
    
    
    vec4 tpos =  M * vec4(vertPos, 1.0);
//    tpos.z += height;
    vertex_pos = tpos.xyz;
    
    gl_Position = P * V * tpos;
    
    vertex_tex = vertTex;
}
