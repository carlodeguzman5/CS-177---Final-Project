#version 120

varying vec4 out_color;
varying vec3 out_worldPos;
varying vec3 out_norm;
uniform vec3 lightPos0;
uniform vec3 lightIntensity0;
uniform vec3 lightPos1;
uniform vec3 lightIntensity1;
uniform vec3 lightPos2;
uniform vec3 lightIntensity2;
uniform vec3 lightAmbient;
//uniform sampler2D texUnit0;
//varying vec2 out_texCoord0;
void main() {
	
	gl_FragColor.rgb = out_color.rgb 
	* ((lightIntensity0 * max(0,dot(normalize(lightPos0 - out_worldPos), normalize(out_norm)))) 
	+ (lightIntensity1 * max(0,dot(normalize(lightPos1 - out_worldPos), normalize(out_norm)))) 
	+ (lightIntensity2 * max(0,dot(normalize(lightPos2 - out_worldPos), normalize(out_norm)))) 
	+ lightAmbient);

	gl_FragColor.a = out_color.a;
	
	//gl_FragColor = mix(gl_FragColor, texture2D(texUnit0, out_texCoord0), gl_FragColor.a);
	//gl_FragColor = texture2D(texUnit0, out_texCoord0);
}