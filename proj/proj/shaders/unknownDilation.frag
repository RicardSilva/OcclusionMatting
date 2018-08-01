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
	
						
	for(int i = -3; i <= 3; i++) {
		for(int j = -3; j <= 3; j++) {
			vec4 trimapColor = texture(trimapEdge, coords + vec2(i * offsetX, j * offsetY));
			if(trimapColor.a < 1) {
				vec4 label = texture(unknownLabels, coords + vec2(i * offsetX, j * offsetY));
				if(label == vec4(1,0,0,1)) { //front half space -> RED 
					vec2 unknownPixelDirection = vec2(trimapColor.r * 2 - 1, trimapColor.g * 2 - 1);
					if(dot(unknownPixelDirection, -vec2(i, j)) >= 0) {
						//frontHalf
						
					}
					else {
						//backHalf -> do nothing
						return true;
					}
				} 
				else if(label == vec4(0,1,0,1)) { //back half space -> GREEN
					vec2 unknownPixelDirection = vec2(trimapColor.r * 2 - 1, trimapColor.g * 2 - 1);
					if(dot(unknownPixelDirection, -vec2(i, j)) >= 0) {
						//frontHalf -> do nothing
						return true;
					}
					else {
						//backHalf
						
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
	
	vec2 offsets[25] = vec2[25](vec2(-2 * offsetX, -2 * offsetY),
						vec2(-2 * offsetX, -1 * offsetY),
						vec2(-2 * offsetX, 0 * offsetY),
						vec2(-2 * offsetX, 1 * offsetY),
						vec2(-2 * offsetX, 2 * offsetY),
						vec2(-1 * offsetX, -2 * offsetY),
						vec2(-1 * offsetX, -1 * offsetY),
						vec2(-1 * offsetX, 0 * offsetY),
						vec2(-1 * offsetX, 1 * offsetY),
						vec2(-1 * offsetX, 2 * offsetY),
						vec2(0 * offsetX, -2 * offsetY),
						vec2(0 * offsetX, -1 * offsetY),
						vec2(0 * offsetX, 0 * offsetY),
						vec2(0 * offsetX, 1 * offsetY),
						vec2(0 * offsetX, 2 * offsetY),
						vec2(1 * offsetX, -2 * offsetY),
						vec2(1 * offsetX, -1 * offsetY),
						vec2(1 * offsetX, 0 * offsetY),
						vec2(1 * offsetX, 1 * offsetY),
						vec2(1 * offsetX, 2 * offsetY),
						vec2(2 * offsetX, -2 * offsetY),
						vec2(2 * offsetX, -1 * offsetY),
						vec2(2 * offsetX, 0 * offsetY),
						vec2(2 * offsetX, 1 * offsetY),
						vec2(2 * offsetX, 2 * offsetY));
						
	vec2 directions[25] = vec2[25] (vec2(-2,-2),
									vec2(-2,-1),
									vec2(-2,0),
									vec2(-2,1),
									vec2(-2,2),
									vec2(-1,-2),
									vec2(-1,-1),
									vec2(-1,0),
									vec2(-1,1),
									vec2(-1,2),
									vec2(0,-2),
									vec2(0,-1),
									vec2(0,0),
									vec2(0,1),
									vec2(0,2),
									vec2(1,-2),
									vec2(1,-1),
									vec2(1,0),
									vec2(1,1),
									vec2(1,2),
									vec2(2,-2),
									vec2(2,-1),
									vec2(2,0),
									vec2(2,1),
									vec2(2,2));
									
	int edgePointsCounter = 0;

	for(int i = 0; i < 25; i++) {
		vec4 trimapColor = texture(trimapEdge, coords + offsets[i]);
		if(trimapColor.a < 1) {
			return true;
			}
	}
	
	//no unknown pixels in search region or not in relevant half
	return false;

}

bool expandUnknown3(vec2 coords) {
for(int i = -5; i <= 5; i++) {
	for(int j = -5; j <= 5; j++) {
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
