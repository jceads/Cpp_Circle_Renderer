#version 330 core

layout (location=0) in vec3 inPos;

uniform mat3 uMtxTransform;
void main()
{
    gl_Position = vec4(uMtxTransform*inPos, 1.0);

}