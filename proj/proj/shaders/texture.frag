#version 330

uniform sampler2D textureMap;

in Data {
	vec2 texC;
} DataIn;

out vec4 colorOut;


void main() {

	vec4 color = texture(textureMap, DataIn.texC);
	
	colorOut = vec4(color.xyz, 0.7);
	
	
}