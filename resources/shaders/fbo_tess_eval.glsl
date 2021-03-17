#version 330 core


out vec2 fragTex;
out vec2 TexCoords;

in vec2 fragTex1[];
in vec2 TexCoords1[];

void main(){
    fragTex=fragTex1[0];
    TexCoords= TexCoords1[0];
}

