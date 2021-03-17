#version 330 core
layout(location = 0) out vec4 color;
layout(location = 1) out vec4 colorBW;
layout(location = 2) out vec4 bloom;
in vec3 vertex_normal;
in vec3 vertex_pos;
in vec2 vertex_tex;
uniform vec3 campos;
uniform float isSUN;
in float isBW_;
uniform sampler2D tex;
void main()
{
vec4 tcol = texture(tex, vertex_tex*10., 2);
    color = vec4(1, 1, 0,1);
    
    
 
    
    if (isSUN == 1.0){
        colorBW = vec4(1.0f, 1.0f, 1.0f, 1.0f);
        bloom = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    }else{
        colorBW = vec4(0.0f, 0.0f, 0.0f, 1.0f);
        bloom = vec4(1.0f, 1.0f, 0.0f, 1.0f);
    }
    
    
    float brightness = dot(color.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        bloom = vec4(color.rgb, 1.0);
    else
        bloom = vec4(1.0, 0.0, 0.0, 1.0);
//    bloom = vec4(1.0f, 1.0f, 0.0f, 1.0f);
}
