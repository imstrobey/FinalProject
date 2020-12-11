#version 410 core

uniform mat4 mvpMatrix;

layout(location = 0) in vec3 vPos;
//uniform vec3 topperColor;
//out vec3 topperFragColor;

void main() {
    gl_Position = mvpMatrix * vec4(vPos,1.0);
//    topperFragColor = topperColor;
}
