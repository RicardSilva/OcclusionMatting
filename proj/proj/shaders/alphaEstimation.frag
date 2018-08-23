#version 330


in vec2 texC;

layout(location = 0) out vec4 colorOut;


uniform sampler2D virtualColor;
uniform sampler2D realColor;
uniform sampler2D finalTrimap;
uniform sampler2D expandedForeground;
uniform sampler2D expandedBackground;
uniform sampler2D propagationCostsForeground;
uniform sampler2D propagationCostsBackground;

uniform sampler2D realSmoothDepth;


uniform sampler2D coarseTrimap;
uniform sampler2D trimapEdge;
uniform sampler2D realColorEdge;

uniform sampler2D unknownLabels;

const float resX = 1920.0;
const float resY = 1080.0;

const float offsetX = 1.0 / resX;
const float offsetY = 1.0 / resY;

const vec2 offsets[9] = vec2[9] (
		vec2(-offsetX,offsetY),
		vec2(-offsetX,0),
		vec2(-offsetX,-offsetY),
		vec2(0,offsetY),
		vec2(0,0),
		vec2(0,-offsetY),
		vec2(offsetX,offsetY),
		vec2(offsetX,0),
		vec2(offsetX,-offsetY));
		

float alphaEstimation(vec4 foregroundColor, vec4 backgroundColor, vec4 pixelColor) {
	float l = length(foregroundColor.rgb - backgroundColor.rgb);
	float estimation = dot((pixelColor.rgb - backgroundColor.rgb), 
			 (foregroundColor.rgb - backgroundColor.rgb))
			/ (l * l);
	if(estimation > 0.2) estimation = 1 ;
	return estimation;
}

float colorCost(vec4 foregroundColor, vec4 backgroundColor,
				vec4 pixelColor) {
					
	float alpha = alphaEstimation(foregroundColor, backgroundColor, pixelColor);
	
	return length(pixelColor 
		- (alpha * foregroundColor 
			+ (1-alpha) * backgroundColor));
}

float propagationCost(vec4 foregroundColor, vec4 backgroundColorn, int i) {
	float dm = 5.0; //FIX ME
	return (texture(propagationCostsForeground, texC + offsets[i]).r * 10
	+ texture(propagationCostsBackground, texC + offsets[i]).r * 10 ) / 2.0f * dm;
}

float objectiveFunction(vec4 foregroundColor, vec4 backgroundColor, vec4 pixelColor, int i) {
	//find min of sum of color cost with propagation cost around the pixel
	float weight = 2f;
	return weight * colorCost(foregroundColor, backgroundColor, pixelColor) 
		+   propagationCost(foregroundColor, backgroundColor, i);
	
	
}


float computeAlpha() {
	//find best sample pair among neighbors using objectiveFunction
	vec4 pixelColor = texture(realColor, texC);
	
	
	vec4 foregroundSamples [9] = vec4[9](
		texture(expandedForeground, texC + offsets[0]),
		texture(expandedForeground, texC + offsets[1]),
		texture(expandedForeground, texC + offsets[2]),
		texture(expandedForeground, texC + offsets[3]),
		texture(expandedForeground, texC),
		texture(expandedForeground, texC + offsets[5]),
		texture(expandedForeground, texC + offsets[6]),
		texture(expandedForeground, texC + offsets[7]),
		texture(expandedForeground, texC + offsets[8]));
		
	vec4 backgroundSamples [9] = vec4[9](
		texture(expandedBackground, texC + offsets[0]),
		texture(expandedBackground, texC + offsets[1]),
		texture(expandedBackground, texC + offsets[2]),
		texture(expandedBackground, texC + offsets[3]),
		texture(expandedBackground, texC),
		texture(expandedBackground, texC + offsets[5]),
		texture(expandedBackground, texC + offsets[6]),
		texture(expandedBackground, texC + offsets[7]),
		texture(expandedBackground, texC + offsets[8]));
	
	float minimumCost = -10000;
	float cost = 0;
	int minimumSampleIndex = 0;
	
	for(int i = 0; i < 9; i++) {
		cost = objectiveFunction(foregroundSamples[i], backgroundSamples[i], pixelColor, i);
		if(cost < minimumCost) {
			minimumCost = cost;
			minimumSampleIndex = i;
		}
		
	}
		
	//estimate alpha value
	
	return alphaEstimation(foregroundSamples[minimumSampleIndex], backgroundSamples[minimumSampleIndex], pixelColor);
	
}




void main() {
	
	float alpha = 0;
	
	vec4 trimapColor = texture(finalTrimap, texC);
	if(trimapColor == vec4(1,0,0,1)) {//RED -> REAL COLOR
		colorOut = texture(realColor, texC);
		alpha = 0;
	}
	else if (trimapColor == vec4(1,1,1,1)) { //WHITE -> REAL COLOR
		colorOut = texture(realColor, texC);
		alpha = 0;
	}
	else if (trimapColor == vec4(0,0,0,1)){ //BLACK -> VIRTUAL COLOR
		colorOut = texture(virtualColor, texC);
		alpha = 1;
	}
	 if (trimapColor.a < 1) {	//UNKNOWN -> compute best color
		alpha = computeAlpha();
		colorOut = alpha * texture(realColor , texC) + (1 - alpha) * texture(virtualColor, texC);
		//colorOut = vec4(0,1,0,1);
	}
	
	//colorOut = vec4(alpha, alpha,alpha, 1);
	//colorOut = texture(finalTrimap, texC) * 0.5;
	//if(texture(trimapEdge, texC).a < 1) colorOut = vec4(1,0,0,1);
	//colorOut += vec4(0, texture(realColorEdge, texC).r + texture(realColorEdge, texC).g + texture(realColorEdge, texC).b ,0,1 );
	//colorOut = texture(expandedForeground, texC);
	//colorOut = texture(expandedBackground, texC);
	
	//colorOut = texture(realColor, texC);
	//colorOut = vec4(texture(realSmoothDepth, texC).rrr, 1);
	
	//colorOut.a =1 ;
}


