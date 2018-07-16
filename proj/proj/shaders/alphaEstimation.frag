#version 330


in vec2 texC;

layout(location = 0) out vec4 colorOut;


uniform sampler2D virtualColor;
uniform sampler2D virtualDepth;
uniform sampler2D realColor;
uniform sampler2D realDepth;
uniform sampler2D realSmoothDepth;
uniform sampler2D finalTrimap;
uniform sampler2D expandedForeground;
uniform sampler2D expandedBackground;

const float zNear = 0.1;
const float zFar = 8000.0;

const int INVALID = -1;
const int UKNOWN = 2;
const int BACKGROUND = 1;
const int FOREGROUND = 0;

const float resX = 1920.0;
const float resY = 1080.0;

const float offsetX = 1.0 / resX;
const float offsetY = 1.0 / resY;

float alphaEstimation(vec4 foregroundColor, vec4 backgroundColor, vec4 pixelColor) {
	float l = length(foregroundColor - backgroundColor);
	return((pixelColor - backgroundColor) 
			* (foregroundColor - backgroundColor)) 
			/ (l * l);
}

float colorCost(vec4 foregroundColor, vec4 backgroundColor,
				vec4 pixelColor, float alpha) {
	
	return length(pixelColor 
		- (alpha * foregroundColor 
			+ (1-alpha) * backgroundColor));
}

float propagationCost(vec4 foregroundColor, vec4 backgroundColor) {
	float dm = 10.0; //FIX ME
	return (d(foregroundColor) + d(backgroundColor)) / 2.0f * dm;
}

float objectiveFunction(vec4 foregroundColor, vec4 backgroundColor, vec4 pixelColor) {
	//find min of sum of color cost with propagation cost around the pixel
	float weight = 0.5f;
	return weight * colorCost(foregroundColor, backgroundColor, pixelColor) 
		+ propagationCost(foregroundColor, backgroundColor);
	
	
}


float computeAlpha() {
	//find best sample pair among neighbors using objectiveFunction
	vec4 pixelColor = vec4(); //FIX ME: WHAT IS Ip SUPPOSED TO BE?
	
	
	vec4 foregroundSamples [9] = vec4[9](
		texture(expandedForeground, texC + vec2(-offsetX,offsetY)),
		texture(expandedForeground, texC + vec2(-offsetX,0)),
		texture(expandedForeground, texC + vec2(-offsetX,-offsetY)),
		texture(expandedForeground, texC + vec2(0,offsetY)),
		texture(expandedForeground, texC),
		texture(expandedForeground, texC + vec2(0,-offsetY)),
		texture(expandedForeground, texC + vec2(offsetX,offsetY)),
		texture(expandedForeground, texC + vec2(offsetX,0)),
		texture(expandedForeground, texC + vec2(offsetX,-offsetY)));
		
	vec4 backgroundSamples [9] = vec4[9](
		texture(expandedBackground, texC + vec2(-offsetX,offsetY)),
		texture(expandedBackground, texC + vec2(-offsetX,0)),
		texture(expandedBackground, texC + vec2(-offsetX,-offsetY)),
		texture(expandedBackground, texC + vec2(0,offsetY)),
		texture(expandedBackground, texC),
		texture(expandedBackground, texC + vec2(0,-offsetY)),
		texture(expandedBackground, texC + vec2(offsetX,offsetY)),
		texture(expandedBackground, texC + vec2(offsetX,0)),
		texture(expandedBackground, texC + vec2(offsetX,-offsetY)));
	
	float minimumCost = -10000;
	float cost = 0;
	int minimumSampleIndex = 0;
	
	for(int i = 0; i < 9; i++) {
		cost = objectiveFunction(foregroundSamples[i], backgroundSamples[i], pixelColor);
		if(cost < minimumCost) {
			minimumCost = cost;
			minimumSampleIndex = i;
		}
		
	}
		
	//estimate alpha value
	
	return alphaEstimation(foregroundSamples[minimumSampleIndex], backgroundSamples[minimumSampleIndex], pixelColor);
	
}




void main() {
		
	vec4 trimapColor = texture(finalTrimap, texC);
	if(trimapColor == vec4(1,0,0,1)) {//RED -> REAL COLOR
		colorOut = texture(realColor, texC);
	}
	else if (trimapColor == vec4(1,1,1,1)) { //WHITE -> REAL COLOR
		colorOut = texture(realColor, texC);
	}
	else if (trimapColor == vec4(0,0,0,1)){ //BLACK -> VIRTUAL COLOR
		colorOut = texture(virtualColor, texC);
	}
	else if (trimapColor.a < 1) {	//UNKNOWN -> compute best color
		float alpha = computeAlpha();
		colorOut = alpha * texture(realColor, texC) + (1 - alpha) * texture(virtualColor, texC);
	}
	
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