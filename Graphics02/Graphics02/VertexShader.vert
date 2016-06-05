#version 110 

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

attribute vec4 vertexPosition;
attribute vec4 vertexColor;
attribute vec4 vertexNormal;

varying vec4 fragColor;
varying vec4 fragNormal;
varying vec4 fragPosition;
varying mat4 rotationMatrix;


void main () {
    vec4 Color   = vertexColor;
    fragColor    = Color;
	rotationMatrix = projectionMatrix * viewMatrix * modelMatrix;
	fragNormal   = vertexNormal;
	fragPosition = viewMatrix * modelMatrix * vertexPosition;
    gl_Position  = projectionMatrix * fragPosition;
}