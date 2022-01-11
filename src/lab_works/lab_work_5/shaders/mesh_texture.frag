#version 450

layout( location = 0 ) out vec4 fragColor;
layout( binding = 1 ) uniform sampler2D uDiffuseMap;
layout( binding = 2 ) uniform sampler2D uAmbientMap;
layout( binding = 3 ) uniform sampler2D uSpecularMap;
layout( binding = 4 ) uniform sampler2D uShininessMap;
layout( binding = 5 ) uniform sampler2D uNormalMap;

in vec3 normal;
in vec3 rayon;
in vec2 uv;
in vec3 pTangeant;

uniform vec3 uAmbient;
uniform vec3 uDiffuse;
uniform vec3 uSpecular;

uniform float uShininess;

uniform bool uHasDiffuseMap;
uniform bool uHasAmbientMap;
uniform bool uHasSpecularMap;
uniform bool uHasShininessMap;
uniform bool uHasNormalMap;

void main()
{

	float alpha = uHasDiffuseMap ? texture(uDiffuseMap, uv).w : 1.f;
	if(alpha < 0.5)
		discard;

	vec3 w0;
	vec3 n;
	if( uHasNormalMap ){
		w0 = normalize(pTangeant);
		 n = normalize(texture(uNormalMap, uv).xyz * 2.0 - 1.0);
	}else{
		w0 = normalize(rayon);
		 n = normalize(normal);
	}

	
	float Ia = 1.f;
	vec3 Ca = (uHasAmbientMap ? texture(uAmbientMap, uv).xyz : uAmbient);

	float Id = 1.f;
	vec3 uDiff = uHasDiffuseMap ? texture(uDiffuseMap, uv).xyz : uDiffuse;
	vec3 Cd = max(dot(-w0, n), 0.f) * uDiff;			//Kd*max(cos(theta), 0)

	float Is = 1.f;
	vec3 uSpec = uHasSpecularMap ? texture(uSpecularMap, uv).xxx : uSpecular;
	float uShini = uHasShininessMap ? texture(uShininessMap, uv).x : uShininess;
	vec3 Cs = pow(max(dot(w0, reflect(-w0, n)), 0.f), uShini) * uSpec;

	


	vec3 aColor = Ca * Ia + Cd * Id + Cs * Is ;

	fragColor =  vec4( aColor, alpha);
}
