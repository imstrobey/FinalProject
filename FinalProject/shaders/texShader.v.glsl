#version 410 core

uniform mat4 mvpMatrix;

layout(location = 0) in vec3 vPos;
in vec2 vTexCoord;
out vec2 texCoord;

void main() {

    gl_Position = mvpMatrix * vec4(vPos,1.0);
    texCoord = vTexCoord;
}
