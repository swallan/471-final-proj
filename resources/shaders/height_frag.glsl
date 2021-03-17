#version 330 core
layout(location = 0) out vec4 color;
layout(location = 1) out vec4 colorBW;

in vec3 vertex_pos;

uniform vec3 campos;

in vec2 frag_tex;
in vec3 vertex_normal;
uniform sampler2D tex;

void main()
{
    color = vec4(.2,1,.2,1);
    
    float len = length(vertex_pos.xz + campos.xz);
    len -= 45;
    len /= 4;
    len = clamp(len, 0, 1);
    color.a = 1-len;
    
    
    color.rgb = texture(tex, frag_tex*100).rgb;
    
    
    vec3 n = normalize(vertex_normal);
    vec3 lp = vec3(200.0f, 35.0f, -30.0f);
    vec3 ld = normalize( lp -vertex_pos );
    float d = dot(ld, n);
    d = pow(d, 5);
    d = clamp(d, .5, 1);
    
    color.rgb *= d;
//
//     specular light
    vec3 cd = normalize( - campos-(vertex_pos) );
    vec3 h = normalize(cd + ld);
    float s = dot(h, n);
    s = clamp(s, 0, 1);
    s = pow(s, 100);
    color.rgb += vec3(1,1,1) * s ;
    
    
    
    colorBW = vec4(0, 0, 0, 1);
}
