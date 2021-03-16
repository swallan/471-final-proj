
#version 330 core
layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec3 vertNor;
layout(location = 2) in vec2 vertTex;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

out vec3 vertex_pos;
out vec3 vertex_normal;
out vec2 vertex_tex;
in float isBW;
out float isBW_;
void main()
{
//    int divisor = 100;
//    vec2 textcoords=vertTex;
//    float t = 1.0/100;
////    textcoords -= vec2(camoff.x, camoff.z)*t;
//
//
//    float g = .01;
//    float h = vec3(texture(tex, textcoords/divisor)).r;
//    float h1 = vec3(texture(tex, textcoords/divisor + vec2(0.0, g))).r;
//    float h2 = vec3(texture(tex, textcoords/divisor + vec2(g, 0.0))).r;
//    vec3 a, b, c;
//    a = vec3(0, h * 2, 0);
//    b = vec3(1, h1 * 2, 0);
//    c = vec3(0, h2 * 2, 1);
//
//    vec3 n = - normalize(cross(a -b , a -c ));
//    vertex_normal = n;
    
    
    
    isBW_ = isBW;
    vertex_normal = vec4(M * vec4(vertNor,0.0)).xyz;
    vec4 tpos =  M * vec4(vertPos, 1.0);
    vertex_pos = tpos.xyz;
    gl_Position = P * V * tpos;
    vertex_tex = vertTex;
}
