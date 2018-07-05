#version 330


in vec2 texC;

layout(location = 0) out vec4 colorOut;


uniform sampler2D virtualColor;
uniform sampler2D virtualDepth;
uniform sampler2D realColor;
uniform sampler2D realDepth;
uniform sampler2D realSmoothDepth;
uniform sampler2D coarseTrimap;
uniform sampler2D trimapEdge;
uniform sampler2D realColorEdge;
uniform sampler2D unknownLabels;

const float zNear = 0.1;
const float zFar = 8000.0;


const float resX = 1920.0;
const float resY = 1080.0;

const float offsetX = 1.0 / resX;
const float offsetY = 1.0 / resY;



void main() {
			
	
	colorOut = vec4(0,0,0,1);
	
	vec4 trimapColor = texture(trimapEdge, texC);

	if(trimapColor == vec4(1,0,0,1)){ //invalid area
		colorOut = trimapColor;
	}	
	else if(trimapColor.a < 1) { //already unknown 
		colorOut = vec4(0.5,0.5,0.5,0.9); // unknown -> grey
	}
	else {
		if(expandUnknown(texC) == true) {
			colorOut = vec4(0.5,0.5,0.5,0.9); // unknown -> grey
		}
		else {
			colorOut = trimapColor; // else keep foreground or background
		}
		
	}
	
	
}
