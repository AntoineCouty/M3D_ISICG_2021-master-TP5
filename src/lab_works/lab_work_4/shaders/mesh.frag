#version 450

layout( location = 0 ) out vec4 fragColor;

in vec3 normal;
in vec3 rayon;

uniform vec3 uAmbient;
uniform vec3 uDiffuse;
uniform vec3 uSpecular;

uniform float uShininess;


void main()
{
	vec3 n = normalize(normal);
	vec3 w0 = normalize(rayon);
	if(dot(n, w0) > 0){
		n = -n;
	}


	float Ia = 1.f;
	vec3 Ca = uAmbient ;

	float Id = 1.f;
	vec3 Cd = max(dot(-w0, n), 0.f) * uDiffuse;										//Kd*max(cos(theta), 0)

	float Is = 1.f;
	vec3 Cs = pow(max(dot(-w0, reflect(w0, n)), 0.f), uShininess) * uSpecular;


	vec3 aColor = Ca * Ia + Cd * Id + Cs * Is;

	fragColor =  vec4( aColor, 1.0);
}
