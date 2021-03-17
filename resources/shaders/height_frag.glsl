#version 330 core
out vec4 color;

in vec2 frag_tex;
void main()
{
color = vec4(1,1,1,1);
color.rg = frag_tex;
}

//#version 330 core
//layout(location = 0) out vec4 color;
//layout(location = 1) out vec4 colorBW;
//
//in vec3 vertex_pos;
//in vec2 frag_tex;
//in vec3 vertex_normal;
//
//uniform vec3 campos;
//uniform vec3 camoff;
//in vec3 cpvec;
//uniform sampler2D tex;
//uniform sampler2D tex2;
//
////in float isBW;
//void main()
//{
//
//
//    vec2 textcoords=frag_tex;
//    float t = 1.0/100.;
//    textcoords -= vec2(camoff.x, camoff.z) * t;
//
//    vec3 heightcolor = texture(tex, textcoords*100).rgb;
//
//    heightcolor.r = .1 + heightcolor.r * .9;
//
//    vec3 texturecolor = texture(tex2, textcoords *100).rgb;// * heightcolor.r;
////
//    color.rgb = texturecolor;
//
//
//    vec3 n = normalize(vertex_normal);
//    vec3 lp = vec3(200.0f, 35.0f, -30.0f);
//    vec3 ld = normalize( lp -vertex_pos );
//    float d = dot(ld, n);
//    d = pow(d, 5);
//    d = clamp(d, .5, 1);
//
//    color.rgb *= d;
////
////     specular light
//    vec3 cd = normalize( - campos-(vertex_pos) );
//    vec3 h = normalize(cd + ld);
//    float s = dot(h, n);
//    s = clamp(s, 0, 1);
//    s = pow(s, 100);
//    color.rgb += vec3(1,1,1) * s ;
//
//    float ave = (color.r + color.g + color.b) / 3.0f;
//
//
//    color.rgb *=heightcolor.r;
//
//
//
//
//    float heightOffset = (clamp(vertex_pos.y, 0, .5));
//
//    color.rgb = color.rgb + vec3(1.0f)*heightOffset;
//
//
//
//    float len = length(vertex_pos.xzy + campos.xzy);
//    len -= 45;
//    len /= 4;
//    len = clamp(len, 0, 1);
//    color.a = 1-len;
//
//
//    colorBW = vec4(0, 0, 0, 1);
//
//
//}
