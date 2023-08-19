#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main(){
    FragColor = texture(ourTexture, TexCoord);

    //    if(customFragColor != 0){
    //        fragColor = uColor;
    //    }else{
    //    fragColor = vec4(1.0,0.5,0.5,1.0);
    //    }

}