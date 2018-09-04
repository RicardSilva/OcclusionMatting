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


bool expandUnknown(vec2 coords) {
	
	int dilationWindow = 18;
	int noEdgeCounter = 0;
	for(int k = -5; k <= 5; k++) {
		for(int l = -5; l <= 5; l++) {
			if(texture(unknownLabels, coords + vec2(k * offsetX, l * offsetY)) == vec4(0,0,1,1)) {
				noEdgeCounter++;
			}
			
		}
	}
	if(noEdgeCounter >= 3) dilationWindow = 25;
	
						
	for(int i = -dilationWindow; i <= dilationWindow; i++) {
		for(int j = -dilationWindow; j <= dilationWindow; j++) {
			vec4 trimapColor = texture(trimapEdge, coords + vec2(i * offsetX, j * offsetY));
			if(trimapColor.a < 1) {
				vec4 label = texture(unknownLabels, coords + vec2(i * offsetX, j * offsetY));
				if(label == vec4(1,0,0,1)) { //front half space -> RED 
					vec2 unknownPixelDirection = vec2(trimapColor.r * 2 - 1, trimapColor.g * 2 - 1);
					if(dot(unknownPixelDirection, -vec2(i, j)) >= 0) {
						//frontHalf
							return true;
					}
					else {
						//backHalf -> do nothing
					}
				} 
				else if(label == vec4(0,1,0,1)) { //back half space -> GREEN
					vec2 unknownPixelDirection = vec2(trimapColor.r * 2 - 1, trimapColor.g * 2 - 1);
					if(dot(unknownPixelDirection, -vec2(i, j)) >= 0) {
						//frontHalf -> do nothing
					}
					else {
						//backHalf
						return true;
					}
				}
				else if(label == vec4(0,0,1,1)) { //no edge -> BLUE
					//do nothing
				}
				
				
			}	
		}
	}
	
	//no unknown pixels in search region or not in relevant half
	return false;

}


bool expandUnknown2(vec2 coords) {
	int windowSize = 2;
for(int i = -windowSize; i <= windowSize; i++) {
	for(int j = -windowSize; j <= windowSize; j++) {
		vec4 trimapColor = texture(trimapEdge, coords + vec2(i * offsetX, j* offsetY));
		if(trimapColor.a < 1) {
			return true;
			}
	}
}
	
	//no unknown pixels in search region or not in relevant half
	return false;
}


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
