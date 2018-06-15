#version 330


in vec2 texC;

layout(location = 0) out vec4 colorOut;


uniform sampler2D virtualColor;
uniform sampler2D virtualDepth;
uniform sampler2D realColor;
uniform sampler2D realDepth;
uniform sampler2D realSmoothDepth;

const float zNear = 0.1;
const float zFar = 8000.0;

const float resX = 1920.0;
const float resY = 1080.0;

const float offsetX = 1.0 / resX;
const float offsetY = 1.0 / resY;


float lowPassFilter3x3(vec2 coords) {
	
	if(texture(realDepth, coords).r == -1) {
		return -1;
	}
	float result = 0.0;
	float counter = 0.0;
	float samples [9] = float[9](
	texture(realDepth, coords + vec2(-offsetX,offsetY)).r,
	texture(realDepth, coords + vec2(-offsetX,0)).r,
	texture(realDepth, coords + vec2(-offsetX,-offsetY)).r,
	texture(realDepth, coords + vec2(0,offsetY)).r,
	texture(realDepth, coords + vec2(0,0)).r,
	texture(realDepth, coords + vec2(0,-offsetY)).r,
	texture(realDepth, coords + vec2(offsetX,offsetY)).r,
	texture(realDepth, coords + vec2(offsetX,0)).r,
	texture(realDepth, coords + vec2(offsetX,-offsetY)).r);
	
	for(int i = 0; i < 9; i++) {
		
		if(samples[i] != -1) {
			result += samples[i] / 9.0;
		}
	}	


	
	return result;
	
}




void main() {
		
	//Sample virtual depth	 
	float z_b = texture(virtualDepth, texC).r;	 
	float depthVirtual = (2.0 * zNear) / (zFar + zNear - z_b * (zFar - zNear));
	
	
	if(depthVirtual > 0.2) { //INVALID
		colorOut = vec4(0,0,0,1);
		return;
	}
	
	
	
	vec2 v_texcoord = vec2(texC.s, 1.0 - texC.t);
	float depth = lowPassFilter3x3(v_texcoord);
	
	colorOut = vec4(depth, depth, depth, 1);
	
	
}