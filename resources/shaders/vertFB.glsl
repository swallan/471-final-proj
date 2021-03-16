#version 410 core
layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec2 VertTex;
//layout(location = 2) in vec2 vertTex;
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
in float raysOn;
out float raysOnn;
out vec2 fragTex;
out vec2 TexCoords;
void main()
{
    raysOnn =raysOn;
//    gl_TexCoord[0] = gl_MultiTexCoord0;
    //gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
    gl_Position = vec4(vertPos.x, vertPos.y, 0.0, 1.0);
    TexCoords = VertTex;
    fragTex = VertTex;

}
