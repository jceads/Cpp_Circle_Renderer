#version 410 core
out vec4 FragColor;

in vec4 ourColor;
in vec2 TexCoord;

uniform vec4 uColor;

uniform vec3 objectColor;
uniform vec3 lightColor;

uniform sampler2D ourTexture;

void main(){
    FragColor = vec4(lightColor * objectColor, 1.0);
    //    FragColor = ourColor;
    //    if(customFragColor != 0){
    //        fragColor = uColor;
    //    }else{
    //    fragColor = vec4(1.0,0.5,0.5,1.0);
    //    }

}