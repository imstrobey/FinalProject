#version 410 core

uniform mat4 mvpMatrix;

layout(location = 0) in vec3 vPos;
in vec3 vNormal;
in float vDisplacement;

void main() {
    gl_Position = mvpMatrix * vec4(vPos + (vNormal * vDisplacement), 1.0);
}
