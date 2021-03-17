#version 410 core

layout (triangles, fractional_even_spacing, cw) in;    //equal_spacing
in vec2 TE_vertex_tex[];
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
out vec3 vertex_normal;

uniform sampler2D tex;


uniform vec3 camoff;
out vec3 frag_tex;

float hash(float n) { return fract(sin(n) * 753.5453123); }
float snoise(vec3 x)
    {
    vec3 p = floor(x);
    vec3 f = fract(x);
    f = f * f * (3.0 - 2.0 * f);

    float n = p.x + p.y * 157.0 + 113.0 * p.z;
    return mix(mix(mix(hash(n + 0.0), hash(n + 1.0), f.x),
        mix(hash(n + 157.0), hash(n + 158.0), f.x), f.y),
        mix(mix(hash(n + 113.0), hash(n + 114.0), f.x),
            mix(hash(n + 270.0), hash(n + 271.0), f.x), f.y), f.z);
    }
float noise(vec3 position, int octaves, float frequency, float persistence) {
    float total = 0.0;
    float maxAmplitude = 0.0;
    float amplitude = 1.0;
    for (int i = 0; i < octaves; i++) {
        total += snoise(position * frequency) * amplitude;
        frequency *= 2.0;
        maxAmplitude += amplitude;
        amplitude *= persistence;
        }
    return total / maxAmplitude;
    }


//vec3 calculateNormal(vec3 p1) {
//    float delta = 0.5f;
//
//    vec3 p2 = (p1 + vec3(delta, 0.0f, 0.0f)) * vec3(1.0f, 0.0f, 1.0f);
//    vec3 p3 = (p1 + vec3(0.0f, 0.0f, -delta)) * vec3(1.0f, 0.0f, 1.0f);
//
//    p2.y = getHeight(p2);
//    p3.y = getHeight(p3);
//
//    vec3 u = p2 - p1;
//    vec3 v = p3 - p1;
//
//    vec3 normal = vec3(0.0f);
//    normal.x = (u.y * v.z) - (u.z * v.y);
//    normal.y = (u.z * v.x) - (u.x * v.z);
//    normal.z = (u.x * v.y) - (u.y * v.x);
//
//    return normalize(normal);
//}

void main()
{
    vec4 pos = (gl_TessCoord.x * gl_in[0].gl_Position +
                   gl_TessCoord.y * gl_in[1].gl_Position +
                   gl_TessCoord.z * gl_in[2].gl_Position);

    vec2 Tex = (gl_TessCoord.x * TE_vertex_tex[0] +
                   gl_TessCoord.y * TE_vertex_tex[1] +
                   gl_TessCoord.z * TE_vertex_tex[2]);

    
    int divisor = 100;
    vec2 textcoords=pos.xz;
    float t = 1.0/100;
    textcoords -= vec2(camoff.x, camoff.z)*t;
    
    
    float g = .01;
    float h = vec3(texture(tex, textcoords/divisor)).r;
    float h1 = vec3(texture(tex, textcoords/divisor + vec2(0.0, g))).r;
    float h2 = vec3(texture(tex, textcoords/divisor + vec2(g, 0.0))).r;
    vec3 a, b, c;
    a = vec3(0, h * 2, 0);
    b = vec3(1, h1 * 2, 0);
    c = vec3(0, h2 * 2, 1);
    
    vec3 n = - normalize(cross(a -b , a -c ));
    vertex_normal = n;
    
    
    float height = noise(pos.xzy, 11, 0.03, 0.6);
    float baseheight = noise(pos.xzy, 4, 0.04, 0.3);
    baseheight = pow(baseheight, 5)*3;
    height = baseheight*height;
    height*=60;

    pos.y +=height;


//    frag_tex = Tex;
    gl_Position = P * V * pos;
    
    vec3 texturecolor = texture(tex, pos.xz*100.0).rgb;
    frag_tex = texturecolor;

}
