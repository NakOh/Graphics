#version 110 

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

attribute vec4 vertexPosition;
attribute vec4 vertexColor;

varying vec4 fragColor;

void main () {
    vec4 Color   = vertexColor;
    fragColor    = Color;
    gl_Position  = projectionMatrix * viewMatrix * modelMatrix * vertexPosition;
}