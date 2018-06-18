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



vec4 generateTrimap(vec2 coords) {
	//Sample smoothed real depth
	float depthReal = texture(realSmoothDepth, coords).r;	
	float invalidDepthReal = texture(realSmoothDepth, coords).g;
	
	//Sample virtual depth	 
	float z_b = texture(virtualDepth, coords).r;	 
	float depthVirtual = (2.0 * zNear) / (zFar + zNear - z_b * (zFar - zNear));
	
	
	if(depthVirtual > 0.2) { //INVALID
		return vec4(1,0,0,1);
	}
	else {
		
		if (invalidDepthReal == 1) { //VIRTUAL IN FOREGROUND
			return vec4(0,0,0,1); 
		}
		else if (depthVirtual <= depthReal) { //VIRTUAL IN FOREGROUND
			return vec4(0,0,0,1); 
		}
		
		else if (depthVirtual > depthReal) { //VIRTUAL IN BACKGROUND
			return vec4(1,1,1,1); 
		}
		
		
	
	}
}

/*float sobel3x3(vec2 coords) {
	int gradX = 0;
	int gradY = 0;
	int samples [9] = int[9](
		evaluateLayer(coords + vec2(-offsetX,offsetY)),
		evaluateLayer(coords + vec2(-offsetX,0)),
		evaluateLayer(coords + vec2(-offsetX,-offsetY)),
		evaluateLayer(coords + vec2(0,offsetY)),
		evaluateLayer(coords + vec2(0,0)),
		evaluateLayer(coords + vec2(0,-offsetY)),
		evaluateLayer(coords + vec2(offsetX,offsetY)),
		evaluateLayer(coords + vec2(offsetX,0)),
		evaluateLayer(coords + vec2(offsetX,-offsetY)));
	for(int i = 0; i < 9; i++)
		if(samples[i] == INVALID) return 0;
		

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
	
	
	return sqrt(gradX * gradX + gradY * gradY);
	
}
*/


void main() {
			
	
	colorOut = vec4(0,0,0,1);
	//### Coarse segmentation ###
	colorOut = generateTrimap(texC);	
}








	//RAW DEPTH OCCLUSION
	/*
	float z_b = texture(virtualDepth, texC).r;	 
	float depthV = (2.0 * zNear) / (zFar + zNear - z_b * (zFar - zNear));
	vec4 colorV = texture(virtualColor, texC);
	
	vec2 v_texcoord = vec2(texC.s, 1.0 - texC.t);
	float depthR = texture(realDepth, v_texcoord).r;	
	vec4 colorR = texture(realColor, v_texcoord);

	if(depthR == 1 && depthV < 0.9)
		colorOut = colorV;
	else if(depthR == -1 && depthV < 0.9)
		colorOut = colorR;
	else if(depthR < depthV || depthV > 0.9 )
		colorOut = colorR;
	else
		colorOut = colorV;
	
	*/