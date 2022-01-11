#version 450

layout( location = 0 ) in vec3 aVertexPosition;
layout( location = 1 ) in vec3 aVertexNormal;
layout( location = 2 ) in vec2 aVertexTexCoords;
layout( location = 3 ) in vec3 aVertexTangent;
layout( location = 4 ) in vec3 aVertexBitagent;


uniform mat4 uMVPMatrix;			// Projection * View * Model
uniform mat4 uNormalMatrix; 
uniform mat4 uModelViewMatrix;		// View * Model

out vec3 normal;
out vec3 rayon;
out vec2 uv;
out vec3 pTangeant;

void main()
{	
	vec3 tView =(uNormalMatrix * normalize( vec4(aVertexTangent, 1.f))).xyz;
	vec3 bView = (uNormalMatrix * normalize( vec4(aVertexBitagent, 1.f))).xyz;
	vec3 nView = (uNormalMatrix * normalize(vec4(aVertexNormal, 1.f))).xyz;
	mat3 inv_TBN = transpose(mat3(tView, bView, nView));


	uv = aVertexTexCoords;
	normal = normalize((uNormalMatrix * vec4(aVertexNormal, 0.f)).xyz);							//Application de la NormalMatrix aux normales
	rayon =  normalize((uModelViewMatrix * vec4(aVertexPosition, 1.0)).xyz);			//Calcul de L0 avec les positions misent dans le View space

	pTangeant = inv_TBN * rayon;

	gl_Position = uMVPMatrix * vec4( aVertexPosition , 1.f );

}

