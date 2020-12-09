#version 410 core

uniform sampler2D textureMap;
in vec2 texCoord;

out vec4 fragColorOut;

void main() {

    vec4 texel = texture(textureMap,texCoord);

    fragColorOut = texel;
}
