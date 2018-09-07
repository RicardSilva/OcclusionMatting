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
	float samples [9] = float[9](
		dot(texture(realColor, coords + vec2(-offsetX,offsetY)).rgb,vec3(0.21, 0.72, 0.007)),
		dot(texture(realColor, coords + vec2(-offsetX,0)).rgb,vec3(0.21, 0.72, 0.007)),
		dot(texture(realColor, coords + vec2(-offsetX,-offsetY)).rgb,vec3(0.21, 0.72, 0.007)),
		dot(texture(realColor, coords + vec2(0,offsetY)).rgb,vec3(0.21, 0.72, 0.007)),
		dot(texture(realColor, coords + vec2(0,0)).rgb,vec3(0.21, 0.72, 0.007)),
		dot(texture(realColor, coords + vec2(0,-offsetY)).rgb,vec3(0.21, 0.72, 0.007)),
		dot(texture(realColor, coords + vec2(offsetX,offsetY)).rgb,vec3(0.21, 0.72, 0.007)),
		dot(texture(realColor, coords + vec2(offsetX,0)).rgb,vec3(0.21, 0.72, 0.007)),
		dot(texture(realColor, coords + vec2(offsetX,-offsetY)).rgb,vec3(0.21, 0.72, 0.007)));
	
	
	gradX += samples[0] * 1;
	gradX += samples[1] * 2;
	gradX += samples[2] * 1;
	//gradX += samples[3] * 0;
	//gradX += samples[4] * 0;
	//gradX += samples[5] * 0;
	gradX += samples[6] * -1;
	gradX += samples[7] * -2;
	gradX += samples[8] * -1;
	
	gradY += samples[0] * 1;
	//gradY += samples[1] * 0;
	gradY += samples[2] * -1;
	gradY += samples[3] * 2;
	//gradY += samples[4] * 0;
	gradY += samples[5] * -2;
	gradY += samples[6] * 1;
	//gradY += samples[7] * 0;
	gradY += samples[8] * -1;
	
	
	float magnitude = sqrt(gradX * gradX + gradY * gradY);
	return vec4(magnitude, (clamp(gradX, -1, 1) + 1) / 2, (clamp(gradY, -1, 1) + 1) / 2, 0.9);
	
}
float sobel3x3R(vec2 coords) {
	float gradX = 0;
	float gradY = 0;
	float samples [9] = float[9](
		texture(realColor, coords + vec2(-offsetX,offsetY)).r,
		texture(realColor, coords + vec2(-offsetX,0)).r,
		texture(realColor, coords + vec2(-offsetX,-offsetY)).r,
		texture(realColor, coords + vec2(0,offsetY)).r,
		texture(realColor, coords + vec2(0,0)).r,
		texture(realColor, coords + vec2(0,-offsetY)).r,
		texture(realColor, coords + vec2(offsetX,offsetY)).r,
		texture(realColor, coords + vec2(offsetX,0)).r,
		texture(realColor, coords + vec2(offsetX,-offsetY)).r);
	
	
	gradX += samples[0] * 1;
	gradX += samples[1] * 2;
	gradX += samples[2] * 1;
	//gradX += samples[3] * 0;
	//gradX += samples[4] * 0;
	//gradX += samples[5] * 0;
	gradX += samples[6] * -1;
	gradX += samples[7] * -2;
	gradX += samples[8] * -1;
	
	gradY += samples[0] * 1;
	//gradY += samples[1] * 0;
	gradY += samples[2] * -1;
	gradY += samples[3] * 2;
	//gradY += samples[4] * 0;
	gradY += samples[5] * -2;
	gradY += samples[6] * 1;
	//gradY += samples[7] * 0;
	gradY += samples[8] * -1;
	
	
	//float magnitude = sqrt(gradX * gradX + gradY * gradY);
	//return vec4(magnitude, (clamp(gradX, -1, 1) + 1) / 2, (clamp(gradY, -1, 1) + 1) / 2, 0.9);
	return sqrt(gradX * gradX + gradY * gradY);
	
}
float sobel3x3G(vec2 coords) {
	float gradX = 0;
	float gradY = 0;
	float samples [9] = float[9](
		texture(realColor, coords + vec2(-offsetX,offsetY)).g,
		texture(realColor, coords + vec2(-offsetX,0)).g,
		texture(realColor, coords + vec2(-offsetX,-offsetY)).g,
		texture(realColor, coords + vec2(0,offsetY)).g,
		texture(realColor, coords + vec2(0,0)).g,
		texture(realColor, coords + vec2(0,-offsetY)).g,
		texture(realColor, coords + vec2(offsetX,offsetY)).g,
		texture(realColor, coords + vec2(offsetX,0)).g,
		texture(realColor, coords + vec2(offsetX,-offsetY)).g);
	
	
	gradX += samples[0] * 1;
	gradX += samples[1] * 2;
	gradX += samples[2] * 1;
	//gradX += samples[3] * 0;
	//gradX += samples[4] * 0;
	//gradX += samples[5] * 0;
	gradX += samples[6] * -1;
	gradX += samples[7] * -2;
	gradX += samples[8] * -1;
	
	gradY += samples[0] * 1;
	//gradY += samples[1] * 0;
	gradY += samples[2] * -1;
	gradY += samples[3] * 2;
	//gradY += samples[4] * 0;
	gradY += samples[5] * -2;
	gradY += samples[6] * 1;
	//gradY += samples[7] * 0;
	gradY += samples[8] * -1;
	
	
	//float magnitude = sqrt(gradX * gradX + gradY * gradY);
	//return vec4(magnitude, (clamp(gradX, -1, 1) + 1) / 2, (clamp(gradY, -1, 1) + 1) / 2, 0.9);
	return sqrt(gradX * gradX + gradY * gradY);
	
}
float sobel3x3B(vec2 coords) {
	float gradX = 0;
	float gradY = 0;
	float samples [9] = float[9](
		texture(realColor, coords + vec2(-offsetX,offsetY)).b,
		texture(realColor, coords + vec2(-offsetX,0)).b,
		texture(realColor, coords + vec2(-offsetX,-offsetY)).b,
		texture(realColor, coords + vec2(0,offsetY)).b,
		texture(realColor, coords + vec2(0,0)).b,
		texture(realColor, coords + vec2(0,-offsetY)).b,
		texture(realColor, coords + vec2(offsetX,offsetY)).b,
		texture(realColor, coords + vec2(offsetX,0)).b,
		texture(realColor, coords + vec2(offsetX,-offsetY)).b);
	
	
	gradX += samples[0] * 1;
	gradX += samples[1] * 2;
	gradX += samples[2] * 1;
	//gradX += samples[3] * 0;
	//gradX += samples[4] * 0;
	//gradX += samples[5] * 0;
	gradX += samples[6] * -1;
	gradX += samples[7] * -2;
	gradX += samples[8] * -1;
	
	gradY += samples[0] * 1;
	//gradY += samples[1] * 0;
	gradY += samples[2] * -1;
	gradY += samples[3] * 2;
	//gradY += samples[4] * 0;
	gradY += samples[5] * -2;
	gradY += samples[6] * 1;
	//gradY += samples[7] * 0;
	gradY += samples[8] * -1;
	
	
	//float magnitude = sqrt(gradX * gradX + gradY * gradY);
	//return vec4(magnitude, (clamp(gradX, -1, 1) + 1) / 2, (clamp(gradY, -1, 1) + 1) / 2, 0.9);
	return sqrt(gradX * gradX + gradY * gradY);
	
}



void main() {
			
	
	colorOut = vec4(0,0,0,1);
	
	
	float result = sobel3x3R(texC);
	result += sobel3x3G(texC);	
	result += sobel3x3B(texC);		
	
	if(result < 1.5) 
		colorOut = vec4(0,0,0,1);		
	else
		colorOut = vec4(1, 1, 1, 0.9);
	
	
}
