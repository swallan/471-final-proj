#version 330 core
layout(location = 0) in vec3 vertPos;
layout(location = 2) in vec3 vertNor;
layout(location = 1) in vec2 vertTex;



uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
out vec3 vertex_pos;
out vec2 vertex_tex;

uniform vec3 campos;

uniform vec3 camoff;
out vec3 vertex_normal;

uniform sampler2D tex;
uniform sampler2D tex2;

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

//Changing octaves, frequency and presistance results in a total different landscape.
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



void main()
{
    
    
    
    
    
//    cpvec = vec3(campos);
    
    int divisor = 100;
    vec2 textcoords=vertTex;
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
    
   // float height = noise(pos.xzy, 11, 0.03, 0.6);
   // float baseheight = noise(pos.xzy, 4, 0.004, 0.3);
   // baseheight = pow(baseheight, 5)*3;
   // height = baseheight*height;
   // height*=60
    

    
    
    float heightFromText = 0;//vec3(texture(tex, textcoords)).r;
    
    
    vec4 tpos = vec4(vertPos, 1.0);
    tpos.z -=camoff.z;
    tpos.x -=camoff.x;
    
    
    tpos = M * tpos;
    vec3 diff =  vec3(1000.0f, 0.0f, 10.0f) * -1;
    float height = noise((tpos.xzy *10) + diff , 11, 0.3, 0.006);
    float baseheight = noise((tpos.xzy *10) + diff, 4, 0.004, .0003);
    baseheight = pow(baseheight, 5)*3;
    height =height * baseheight;

    // only use the noise
    height =heightFromText *30.01 +((height) * 10 );
    
    tpos.y += height;
    vertex_pos = tpos.xyz;
    gl_Position = P * V * tpos;
    vertex_tex = vertTex;

}
