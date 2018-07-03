#version 330


in vec2 texC;

layout(location = 0) out vec4 colorOut;


uniform sampler2D virtualColor;
uniform sampler2D virtualDepth;
uniform sampler2D realColor;
uniform sampler2D realDepth;
uniform sampler2D realSmoothDepth;
uniform sampler2D coarseTrimap;

const float zNear = 0.1;
const float zFar = 8000.0;


const float resX = 1920.0;
const float resY = 1080.0;

const float offsetX = 1.0 / resX;
const float offsetY = 1.0 / resY;





vec4 sobel3x3(vec2 coords) {
	float gradX = 0;
	float gradY = 0;
	vec4 samples [9] = vec4[9](
		texture(coarseTrimap, coords + vec2(-offsetX,offsetY)),
		texture(coarseTrimap, coords + vec2(-offsetX,0)),
		texture(coarseTrimap, coords + vec2(-offsetX,-offsetY)),
		texture(coarseTrimap, coords + vec2(0,offsetY)),
		texture(coarseTrimap, coords + vec2(0,0)),
		texture(coarseTrimap, coords + vec2(0,-offsetY)),
		texture(coarseTrimap, coords + vec2(offsetX,offsetY)),
		texture(coarseTrimap, coords + vec2(offsetX,0)),
		texture(coarseTrimap, coords + vec2(offsetX,-offsetY)));
	for(int i = 0; i < 9; i++){
		if(samples[i] == vec4(1,0,0,1)) 
			return vec4(0,0,0,1);
	}
		

	gradX += samples[0].g * 1;
	gradX += samples[1].g * 2;
	gradX += samples[2].g * 1;
	//gradX += samples[3].g * 0;
	//gradX += samples[4].g * 0;
	//gradX += samples[5].g * 0;
	gradX += samples[6].g * -1;
	gradX += samples[7].g * -2;
	gradX += samples[8].g * -1;
	
	gradY += samples[0].g * 1;
	//gradY += samples[1].g * 0;
	gradY += samples[2].g * -1;
	gradY += samples[3].g * 2;
	//gradY += samples[4].g * 0;
	gradY += samples[5].g * -2;
	gradY += samples[6].g * 1;
	//gradY += samples[7].g * 0;
	gradY += samples[8].g * -1;
	
	
	float magnitude = sqrt(gradX * gradX + gradY * gradY);
	return vec4(magnitude, (gradX + 1) / 2, (gradY + 1) / 2, 0.9);
	
}



void main() {
			
	
	colorOut = vec4(0,0,0,1);
	vec4 result = sobel3x3(texC);	
	if(result.a < 0.1)
		colorOut = texture(coarseTrimap, texC);
	else
		colorOut = result;
	
}
