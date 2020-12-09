/*
 *   Geometry Shader
 *
 *   CSCI 441, Computer Graphics, Colorado School of Mines
 */

#version 410 core

// TODO #A - set primitive input type
layout ( points ) in;
// TODO #B - set primitive output type
//layout ( points, max_vertices = 1 ) out;
//layout ( points, max_vertices = 4 ) out;
layout ( triangle_strip, max_vertices = 4 ) out;

uniform mat4 projMatrix;

// TODO #I - output varying variable
out vec2 texCoord;

void main() {
    // TODO #C - set gl_Position in clip space for every vertex
    gl_Position = projMatrix * (gl_in[0].gl_Position + vec4(1,1,1,1));
    texCoord = vec2(0,0);
    // TODO #D - send vertex down pipeline
    EmitVertex();
    // TODO #F
    gl_Position = projMatrix * (gl_in[0].gl_Position + vec4(1,-1,1,1));
    texCoord = vec2(1,0);
    EmitVertex();
    // TODO #G
    gl_Position = projMatrix * (gl_in[0].gl_Position + vec4(-1,1,1,1));
    texCoord = vec2(0,1);
    EmitVertex();
    // TODO #H
    gl_Position = projMatrix * (gl_in[0].gl_Position + vec4(-1,-1,1,1));
    texCoord = vec2(1,1);
    EmitVertex();
    // TODO #E - wrap primitive
    EndPrimitive();
}
