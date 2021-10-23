#version 330 core

in vec3 vertex;
in vec3 color;

uniform mat4 TG;
uniform mat4 Proj;
uniform mat4 View;

uniform int indexColor;

out vec3 fcolor;

vec3 blanc = vec3(0.8, 0.8, 0.8);
vec3 negro = vec3(0.2, 0.2, 0.2);

void main() {
    if (indexColor == 0){
    	fcolor = blanc;
    }
    
    else if (indexColor == 1){
    	fcolor = negro;
    }
    
    else fcolor = color;
    
    gl_Position = Proj * View * TG * vec4 (vertex, 1.0);
}
