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

const float zNear = 0.1;
const float zFar = 8000.0;


const float resX = 1920.0;
const float resY = 1080.0;

const float offsetX = 1.0 / resX;
const float offsetY = 1.0 / resY;


vec4 labelUnknown(vec2 coords) {
	
	vec4 samples [25] = vec4[25](vec4());
	
	for(int i = -2; i < 2; i++) {
		for(int j = -2; j < 2; j++) {
			
		}
	}
	
	
	
	return vec4(0,0,0,1);
}


void main() {
			
	
	colorOut = vec4(0,0,0,1);
	vec2 v_texcoord = vec2(texC.s, 1.0 - texC.t);
	
	vec4 trimapColor = texture(trimapEdge, texC);

	
	//colorOut += texture(trimapEdge, texC) * 0.5;
	//colorOut += texture(realColorEdge, texC) * 0.5;
	
	if(trimapColor != vec4(1,0,0,1) && trimapColor != vec4(1,1,1,1) && trimapColor != vec4(0,0,0,1)) { //unknown edge
		labelUnknown(texC);
	}
	else {
		colorOut = vec4(0,0,0,1);
	}
	
}
