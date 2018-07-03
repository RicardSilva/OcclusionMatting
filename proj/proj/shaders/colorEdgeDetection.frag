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
		dot(texture(realColor, coords + vec2(-offsetX,offsetY)).rgb,vec3(0.299, 0.587, 0.114)),
		dot(texture(realColor, coords + vec2(-offsetX,0)).rgb,vec3(0.299, 0.587, 0.114)),
		dot(texture(realColor, coords + vec2(-offsetX,-offsetY)).rgb,vec3(0.299, 0.587, 0.114)),
		dot(texture(realColor, coords + vec2(0,offsetY)).rgb,vec3(0.299, 0.587, 0.114)),
		dot(texture(realColor, coords + vec2(0,0)).rgb,vec3(0.299, 0.587, 0.114)),
		dot(texture(realColor, coords + vec2(0,-offsetY)).rgb,vec3(0.299, 0.587, 0.114)),
		dot(texture(realColor, coords + vec2(offsetX,offsetY)).rgb,vec3(0.299, 0.587, 0.114)),
		dot(texture(realColor, coords + vec2(offsetX,0)).rgb,vec3(0.299, 0.587, 0.114)),
		dot(texture(realColor, coords + vec2(offsetX,-offsetY)).rgb,vec3(0.299, 0.587, 0.114)));
	
	
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
	return vec4(magnitude, (gradX + 1) / 2, (gradY + 1) / 2, 0.9);
	
}



void main() {
			
	
	colorOut = vec4(0,0,0,1);
	vec2 v_texcoord = vec2(texC.s, 1.0 - texC.t);
	
	
	vec4 result = sobel3x3(v_texcoord);	
	
	if(result.r < 0.35) 
		colorOut = vec4(0,0,0,1);		
	else
		colorOut = result;
	
	
}
