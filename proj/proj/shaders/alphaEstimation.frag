#version 330


in vec2 texC;

layout(location = 0) out vec4 colorOut;


uniform sampler2D virtualColor;
uniform sampler2D realColor;
uniform sampler2D finalTrimap;
uniform sampler2D dilatedTrimap;
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

const float iterations = 5;


float alphaEstimation(vec4 foregroundColor, vec4 backgroundColor, vec4 pixelColor) {
	float l = length(foregroundColor.rgb - backgroundColor.rgb);
	float estimation = (dot((pixelColor.rgb - backgroundColor.rgb),
			(foregroundColor.rgb - backgroundColor.rgb)))
			/ (l * l);
	return estimation ;
}

float colorCost(vec4 foregroundColor, vec4 backgroundColor,
				vec4 pixelColor, float alpha) {				
	
	
	return length(pixelColor.rgb 
		- (alpha * foregroundColor.rgb 
			+ (1-alpha) * backgroundColor.rgb));
}

float propagationCost(float foregroundCost, float backgroundCost) {
	float dm = iterations;
	return (foregroundCost + backgroundCost ) / 2.0f * dm;
}

float objectiveFunction(vec4 foregroundColor, vec4 backgroundColor, vec4 pixelColor, float foregroundCost, float backgroundCost, float alpha) {
	//find min of sum of color cost with propagation cost around the pixel
	
	
	float weight = 0.2f;
	return weight * colorCost(foregroundColor, backgroundColor, pixelColor, alpha) 
	+ propagationCost(foregroundCost, backgroundCost);
	
	
}


float computeAlpha() {
	//find best sample pair among neighbors using objectiveFunction
	vec4 pixelColor = texture(realColor, texC);
	
	
	float minimumCost = 1000000000;
	float cost = 0;
	vec4 foregroundColor;
	vec4 backgroundColor;
	float foregroundCost;
	float backgroundCost;
	vec4 bestForegound = vec4(0,0,0,1);
	vec4 bestBackground = vec4(0,0,0,1);
	float alpha = 0;
	float bestAlpha = 0;
	int windowSize = 4;
	for(int i = -windowSize; i <= windowSize; i++) {
		for(int j = -windowSize; j <= windowSize; j++) {
			foregroundColor = texture(expandedForeground, texC + vec2(i * offsetX, j * offsetY));
			foregroundCost = texture(propagationCostsForeground, texC + vec2(i * offsetX, j * offsetY)).r * 10; 
			for(int k = -windowSize; k <= windowSize; k++) {
				for(int l = -windowSize; l <= windowSize; l++) {
					backgroundColor = texture(expandedBackground, texC + vec2(k * offsetX, l* offsetY));
					backgroundCost = texture(propagationCostsBackground, texC + vec2(k * offsetX, l * offsetY)).r * 10; 
					
					alpha = alphaEstimation(foregroundColor, backgroundColor, pixelColor);
					
					cost = objectiveFunction(foregroundColor, backgroundColor, pixelColor, foregroundCost, backgroundCost, alpha );
					if(cost < minimumCost) {
						minimumCost = cost;
						bestForegound = foregroundColor;
						bestBackground = backgroundColor;
						bestAlpha = alpha;
					}			
					
					
				}
			}
			
			
				
		}		
	}
		
	
	return bestAlpha;

}




void main() {
	
	float alpha = 0;
	
	vec4 trimapColor = texture(finalTrimap, texC);
	if(trimapColor == vec4(1,0,0,1)) {//RED -> REAL COLOR
		//colorOut = texture(realColor, texC);
		alpha = 1;
	}
	else if (trimapColor == vec4(1,1,1,1)) { //WHITE -> REAL COLOR
		//colorOut = texture(realColor, texC);
		alpha = 1;
	}
	else if (trimapColor == vec4(0,0,0,1)){ //BLACK -> VIRTUAL COLOR
		//colorOut = texture(virtualColor, texC);
		alpha = 0;
	}
	 if (trimapColor.a < 1) {	//UNKNOWN -> compute best color
		alpha = computeAlpha();
		//colorOut = alpha * texture(realColor , texC)  + (1 - alpha) * texture(virtualColor, texC) * 1.5;
		
		//colorOut = vec4(0,1,0,1);
	}
    colorOut = vec4(0,0,0,1);
	
	colorOut = texture(expandedForeground, texC);
			
	//colorOut = vec4(alpha, alpha,alpha, 1);
	//colorOut = texture(finalTrimap, texC);
	//if(texture(trimapEdge, texC).a < 1) colorOut = vec4(1,0,0,1);
	colorOut += texture(unknownLabels, texC);
	//colorOut += vec4(texture(realColorEdge, texC)) * 0.5;
	
	//colorOut = texture(expandedBackground, texC);
	//colorOut = texture(dilatedTrimap, texC);
	//colorOut = vec4(0,0,0,1);
	//colorOut = texture(realColor, texC);
	//colorOut = vec4(texture(realSmoothDepth, texC).rrr, 1) * 5;
	
	//colorOut = texture(propagationCostsBackground, texC);
	
	/*if(texC.s < 0.5 && texC.t < 0.5)
		colorOut = texture(expandedForeground, texC * 2);
	 else if(texC.s > 0.5 && texC.t < 0.5 )
		colorOut = texture(expandedBackground, texC * 2);
	else if(texC.s < 0.5 && texC.t > 0.5 )
		if(alpha == 2) colorOut = vec4(0,1,0,1);
		else colorOut = vec4(alpha, alpha,alpha, 1);
	else if(texC.s > 0.5 && texC.t > 0.5 )
		colorOut = texture(realColor, texC * 2);
	*/
}


