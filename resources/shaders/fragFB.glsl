#version 410 core
out vec4 color;
in vec2 fragTex;
uniform sampler2D colorTexure;
uniform sampler2D BWTexure;
uniform sampler2D bloom;

uniform float raysOn;
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

uniform mat4 P1;
uniform mat4 V1;
uniform mat4 M1;

in vec2 TexCoords;
void main()
{
    color = texture(BWTexure, TexCoords.xy);
//    color.rgb = vec3(fragTex, raysOn);
    color.a = 1;
    return;
    
    if (raysOn == 1.0){
        vec4 light_screen_position = P * V * M * vec4(0.0f,0.0f, 1.0f, 1.0f);
        light_screen_position.xy /= light_screen_position.w;
        vec2 lightPositionOnScreen = vec2( light_screen_position.xy / 2.0);
        color = texture(colorTexure, TexCoords.xy);
        color.a = 1;
        int NUM_SAMPLES = 100;

        vec2 deltaTextCoord= vec2(TexCoords.xy - lightPositionOnScreen - vec2(.5, .5));
        vec2 textCoo = fragTex;
        float density=0.9f;
        float weight=6.0f;
        float exposure=0.0034;
        float decay=1.0f;

        deltaTextCoord *= 1.0 / float(NUM_SAMPLES) * density;
        float illuminationDecay = 1.0f;

        vec4 godray = texture(BWTexure, TexCoords.xy);

        for (int i = 0; i < NUM_SAMPLES; i++){
            textCoo -= deltaTextCoord;
            vec4 sampleGR = texture(BWTexure, textCoo);
            sampleGR *= illuminationDecay * weight;
            godray += sampleGR;
            illuminationDecay *= decay;
        }
        godray *= exposure;

        color = (color + godray) /1.0f;
        
        light_screen_position = P1 * V1 * M1 * vec4(0.0f,0.0f, 1.0f, 1.0f);
        light_screen_position.xy /= light_screen_position.w;
        lightPositionOnScreen = vec2( light_screen_position.xy / 2.0);
//        color = texture(colorTexure, TexCoords.xy);
//        color.a = 1;
        NUM_SAMPLES = 100;

        deltaTextCoord= vec2(TexCoords.xy - lightPositionOnScreen - vec2(.5, .5));
        textCoo = fragTex;
        density=0.9f;
        weight=2.0f;
        exposure=0.0024;
        decay=1.0f;

        deltaTextCoord *= 1.0 / float(NUM_SAMPLES) * density;
        illuminationDecay = 1.0f;

        godray = texture(BWTexure, TexCoords.xy);

        for (int i = 0; i < NUM_SAMPLES; i++){
            textCoo -= deltaTextCoord;
            vec4 sampleGR = texture(BWTexure, textCoo);
            sampleGR *= illuminationDecay * weight;
            godray += sampleGR;
            illuminationDecay *= decay;
        }
        godray *= exposure;

//        color = (color + godray) /1.0f;
        
        
    }
    else{
        color.rgb = texture(colorTexure, fragTex ).rgb;
        color.a = 1;
    }
    vec3 bloomColor = texture(bloom,TexCoords, 0).rgb;
    vec3 overlight = vec3(0);
    
    for (int y=-3 ; y<=3; y++){
        for (int x=-3 ; x<=3; x++){
            float f;
            float l = length(vec2(x, y));
            
            l/=4.24;
            l =1.0 -l;
            vec2 textcoords = fragTex + vec2(x,y)*.2;
            vec3 textureColorLOWRES = texture(bloom, TexCoords,0).rgb;
            
            overlight.r += pow(textureColorLOWRES.r, 3)*l;
            overlight.g += pow(textureColorLOWRES.g, 3)*l;
            overlight.b += pow(textureColorLOWRES.b, 3)*l;
        }
    }
    color.rgb = color.rgb + overlight.rgb;
    
    
    
//    float exposure =1;
//    const float gamma = 2.2;
//    vec3 bloomColor = texture(bloom, TexCoords).rgb;
//    color.rgb += bloomColor; // additive blending
//    vec3 result = vec3(1.0) - exp(-color.rgb * exposure);
//    result = pow(result, vec3(1.0 / gamma));
//    color = vec4(result, 1.0);

    
}
