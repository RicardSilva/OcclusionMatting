#version 330


in vec2 texC;

out vec4 colorOut;


uniform sampler2D virtualColor;
uniform sampler2D virtualDepth;
uniform sampler2D realColor;
uniform sampler2D realDepth;




void main() {

	colorOut = texture(virtualColor, texC);
	//colorOut = vec4(0,0,1,1);
		
	
}