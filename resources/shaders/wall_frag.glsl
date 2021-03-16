#version 330 core
//uniform vec3 campos;
uniform sampler2D tex;
layout(location = 0) out vec4 color;
layout(location = 1) out vec4 colorBW;
in vec3 vertex_normal;
in vec3 vertex_pos;
in vec2 vertex_tex;
in vec3 cpvec;
in float isBW;

void main()
{
    
    // diffuse light
    vec3 n = normalize(vertex_normal);
    vec3 lp = vec3(0.0, .50, 0.0);
    vec3 ld = normalize(lp - vertex_pos);
    float d = dot(n, ld);
    d = clamp(d, 0, 1);
    d = pow(d, .5);
//
//     specular light
    vec3 cd = normalize(-cpvec - (vertex_pos));
    vec3 h = normalize(cd + ld);
    float s = dot(h, n);
    s = clamp(s, .5, 1);
    s = pow(s, 200);
    
    vec4 tcol = texture(tex, vertex_tex*10.);
    color = tcol ;
    color.rgb *=d;
    //color.rgb = (cpvec);
    color.rgb += vec3(.2,.2,.2)*s;
//    if (isBW == 1) color = vec4(.3, .3, .3, 1);
    colorBW = vec4(0, 0, 0, 1);
}
