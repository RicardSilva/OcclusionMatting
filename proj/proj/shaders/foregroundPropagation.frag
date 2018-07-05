#version 440


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
uniform sampler2D finalTrimap;

const float zNear = 0.1;
const float zFar = 8000.0;


const float resX = 1920.0;
const float resY = 1080.0;

const float offsetX = 1.0 / resX;
const float offsetY = 1.0 / resY;



void main() {
			
	vec4 trimapColor = texture(finalTrimap, texC);
	if(trimapColor == vec4(1,1,1,1)) { //WHITE ->	FOREGROUND
		colorOut = texture(realColor, vec2(texC.s, 1 - texC.t));
	}
	else {
		colorOut = vec4(0,0,0,0);
	}
	
	
	
}
