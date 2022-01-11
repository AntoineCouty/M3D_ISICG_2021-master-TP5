#version 450

layout( location = 0 ) out vec4 glColor;

uniform float uLuminosite;

in vec3 color;
void main() {

	glColor = vec4(color*uLuminosite ,1.0);
}
