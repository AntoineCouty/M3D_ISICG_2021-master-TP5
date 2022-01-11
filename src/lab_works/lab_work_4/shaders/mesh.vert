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

void main()
{

	normal = normalize((uNormalMatrix * vec4(aVertexNormal, 0.f)).xyz );				//Application de la NormalMatrix aux normales
	rayon =  normalize((uModelViewMatrix * vec4(aVertexPosition, 1.0)).xyz);			//Calcul de L0 avec les positions misent dans le View space

	gl_Position = uMVPMatrix * vec4( aVertexPosition, 1.f );

}

