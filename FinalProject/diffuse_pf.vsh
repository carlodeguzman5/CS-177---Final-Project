#version 120

uniform mat4 viewProjMatrix;
uniform mat4 modelMatrix;
uniform mat4 normMatrix;

attribute vec3 pos;
attribute vec3 normal;
attribute vec4 color;
//attribute vec2 texCoord0;

varying vec4 out_color;
varying vec3 out_worldPos;
varying vec3 out_norm;
//varying vec2 out_texCoord0;

void main() {
	vec4 p = modelMatrix * vec4(pos, 1);
	out_worldPos = p.xyz;
	out_norm = (normMatrix * vec4(normal, 0)).xyz;
	out_color = color;
	//out_texCoord0 = texCoord0;
	
	gl_Position = viewProjMatrix * p;
}