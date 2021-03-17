#version 330 core

in vec2 fragTex[];
in vec2 TexCoords[];

out vec2 fragTex1[];
out vec2 TexCoords1[];


void main(){
    fragTex1=fragTex[0];
    TexCoords1= TexCoords[0];
}
