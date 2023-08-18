#version 330 core
out vec4 fragColor;
in vec3 customFragColor;
void main(){
    if(customFragColor != 0){
        fragColor = vec4(customFragColor,1.0);
    }else{
    fragColor = vec4(1.0,0.5,0.5,1.0);
    }

}