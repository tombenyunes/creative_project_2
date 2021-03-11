#version 120

varying out vec4 v_color;

//in vec4 position;

// these are passed in from OF programmable renderer
uniform mat4 modelViewMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelViewProjectionMatrix;

//uniform vec4 modelColor;
uniform float time;
uniform float amplitude;
uniform float freqTime;
uniform float freqSpace;

uniform float coordinateSpace;

void main (void){
	

	vec4 posModelSpace = gl_Vertex;
	vec4 posWorldSpace = modelMatrix * posModelSpace;
	vec4 posViewSpace = viewMatrix * posWorldSpace;
	vec4 posProjectionSpace = projectionMatrix * posViewSpace;
	posProjectionSpace /= posProjectionSpace.w;

	/*bool animate = true;
	
	vec4 posModelSpace2 = gl_Vertex;
	if (coordinateSpace == 1 && animate) {
		posModelSpace2.y += amplitude * sin(freqTime * time + freqSpace * posModelSpace2.y);
	}
	vec4 posWorldSpace2 = modelMatrix * posModelSpace2;
	if (coordinateSpace == 2 && animate) {
		posWorldSpace2.y += amplitude * sin(freqTime * time + freqSpace * posWorldSpace2.y);
	}	
	vec4 posViewSpace2 = viewMatrix * posWorldSpace2;
	if (coordinateSpace == 3 && animate) {
		posViewSpace2.y += amplitude * sin(freqTime * time + freqSpace * posViewSpace2.y);
	}	
	vec4 posProjectionSpace2 = projectionMatrix * posViewSpace2;
	posProjectionSpace2 /= posProjectionSpace2.w;
	if (coordinateSpace == 4 && animate) {
		posProjectionSpace2.x += (amplitude / 50) * sin(freqTime * time + freqSpace * posProjectionSpace2.y);
	}*/

	//v_color = vec4(1.0, 0.0, 0.0, 1);
	
	
	gl_Position = modelViewProjectionMatrix * gl_Vertex;
}
