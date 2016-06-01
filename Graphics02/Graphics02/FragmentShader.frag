#version 110 

uniform vec4 cameraPosition;
uniform vec4 lightPosition;

varying vec4 fragColor;
varying vec4 fragNormal;
varying vec4 fragPosition;
varying mat4 rotationMatrix;

void main() {
	vec4 L = rotationMatrix * lightPosition - fragPosition;
	vec4 V = cameraPosition - fragPosition;
	vec4 N = fragNormal;

	L.w = 0.0; L = normalize(L);
	V.w = 0.0; V = normalize(V);
	N.w = 0.0; N = normalize(N);

	float ambient = 1.0;
	float diffusion = max(0.0, dot(L, N));
	float specular = max(0.0, dot(((2.0 * dot(L,fragNormal)* N) -L), V));

	float shiness = 90.0;

	specular = max(0.0,pow(specular, shiness));

	float ac = 0.1;
	float dc = 0.8;
	float sc = 0.2;

	vec4 Color = vec4(0.1,0.1,0.9,1.0);

	gl_FragColor = fragColor * (ac * ambient + dc * diffusion + sc * specular);
	//gl_FragColor = fragColor;
}