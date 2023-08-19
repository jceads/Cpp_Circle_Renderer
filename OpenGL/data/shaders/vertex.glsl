#version 330 core

layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aColor;
layout (location=2) in vec2 aTexCoord;


out vec3 ourColor;
out vec2 TexCoord;

uniform mat3 uMtxTransform;
void main()
{
    //    gl_Position = vec4(uMtxTransform*inPos, 1.0);
    gl_Position = vec4(aPos * uMtxTransform, 1.0f);
    ourColor = aColor;
    TexCoord = aTexCoord;
}