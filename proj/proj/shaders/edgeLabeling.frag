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


vec4 labelUnknown(vec4 trimapColor, vec2 coords) {
	
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
	int backHalfCounter = 0;
	int frontHalfCounter = 0;

	for(int i = 0; i < 25; i++) {
		vec4 color = texture(realColorEdge, coords + offsets[i]);
		if(color.a < 1) {
			edgePointsCounter++;
			vec2 unknownPixelDirection = vec2((trimapColor.r * 2) - 1, (trimapColor.g * 2) - 1);
			if(dot(unknownPixelDirection, offsets[i]) >= 0) {
				backHalfCounter++;
				
			}
			else {
				frontHalfCounter++;
			}
			
		}
	}
	
	//no edge -> BLUE
	if(edgePointsCounter < 3) {
		return vec4(0,0,1,1);
	}
	
	if(frontHalfCounter > backHalfCounter) {
		//front half space -> RED (dot product positive)
		return vec4(1,0,0,1);
	}
	else {
		//back half space -> GREEN (dot product negative)
		return vec4(0,1,0,1);
	}

}


void main() {
			
	
	colorOut = vec4(0,0,0,1);
	
	vec4 trimapColor = texture(trimapEdge, texC);
	
	if(trimapColor.a < 1) { //unknown edge
		colorOut = labelUnknown(trimapColor, texC);
	}
	else {
		colorOut = vec4(0,0,0,1);
	}
	
	
}
