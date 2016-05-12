#version 110 

varying vec4 fragColor;

void main() {
	vec4 Color = vec4(0.1,0.1,0.9,1.0);

	gl_FragColor = fragColor;
}