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

const int INVALID = -1;
const int UKNOWN = 2;
const int BACKGROUND = 1;
const int FOREGROUND = 0;

const float resX = 1920.0;
const float resY = 1080.0;

const float offsetX = 1.0 / resX;
const float offsetY = 1.0 / resY;



int evaluateLayer(vec2 coords) {
	//Sample real depth
	//float depthReal = texture(realDepth, v_texcoord).r;	
	float depthReal = texture(realSmoothDepth, coords).r;	
	
	
	//Sample virtual depth	 
	float z_b = texture(virtualDepth, coords).r;	 
	float depthVirtual = (2.0 * zNear) / (zFar + zNear - z_b * (zFar - zNear));
	
	
	if(depthVirtual > 0.2) { //INVALID
		//return vec4(1,0,0,1);
		return INVALID;
	}
	else {
		
		if (depthReal == -1) { //VIRTUAL IN FOREGROUND
			//return vec4(0,0,0,1); 
			return FOREGROUND;
		}
		if (depthVirtual < depthReal) { //VIRTUAL IN FOREGROUND
			//return vec4(0,0,0,1); 
			return FOREGROUND;
		}
		
		else if (depthVirtual > depthReal) { //VIRTUAL IN BACKGROUND
			//return vec4(1,1,1,1) ; 
			return BACKGROUND;
		}
		
		
	
	}
}

float sobel3x3(vec2 coords) {
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
vec4 generateTrimap(vec2 coords) {
	
	int layer = evaluateLayer(coords);
	if(layer == INVALID) //INVALID = RED
		return vec4(1,0,0,1);
	
	float sobelGradient = sobel3x3(coords);
	if(sobelGradient > 0.5) //UKNOWN = GRAY
		//return vec4(0.5,0.5,0.5,1);
		return vec4(0,1,0,1);
	
	else if(layer == FOREGROUND) //FOREGROUND = BLACK
		return vec4(0,0,0,1);
	
	else if(layer == BACKGROUND) //BACKGROUND = WHITE
		return vec4(1,1,1,1);
	
}



void main() {
			
	
	colorOut = vec4(0,0,0,1);
	//### Coarse segmentation ###
	colorOut = generateTrimap(texC);
	
	
	colorOut = texture(virtualDepth, texC);
	//colorOut = vec4(1,0,0,1);
	
	//### Labeling of unknown regions ###
	
	//### Adaptive dilation ###
	
	//====FOREGROUND AND BACKGROUND PROPAGATION====
	//vec4 backgroundColor = vec4();
	//vec4 foregroundColor = vec4();
	
	//====ALPHA ESTIMATION====
	
	//float alpha = computeAlpha();
	//colorOut = alpha * colorVirtual + (1.0f - alpha) * colorReal;
	
	
	
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