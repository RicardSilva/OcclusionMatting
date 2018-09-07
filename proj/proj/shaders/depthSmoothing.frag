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

float depthValues[25];

void bubbleSort(int size) {
	int items_to_sort = size / 2 + 1;
    for(int k = 0; k < items_to_sort; k++) {
        for(int j = 0; j < size - 1 - k; j++) {
            if(depthValues[j] > depthValues[j + 1]) {
                float tmp = depthValues[j];
                depthValues[j] = depthValues[j+1];
                depthValues[j+1] = tmp;
                
            }           
                        
        }
    }
}

vec4 medianFilter5x5(vec2 coords) {
	
	float thisDepth = texture(realDepth, coords).r;
	if(thisDepth == -1.0) {
		return vec4(0,1,0,1);
	}
	int counter = 0;
    for (int i = -2; i <= 2; i++) {
		for (int j = -2; j <= 2; j++) {
			depthValues[counter++] = texture(realDepth, coords + vec2(i * offsetX, j * offsetY)).r;
			
		}
    }

    bubbleSort(25);
	float result = depthValues[12];
	if(result == -1.0) {
		return vec4(0,1,0,1);
	}
		
	return vec4(result, 0,0,1);

}

vec4 lowPassFilter3x3(vec2 coords) {
	float thisDepth = texture(realDepth, coords).r;
	if(thisDepth == -1.0) {
		return vec4(0,1,0,1);
	}
	float result = 0.0;
	float samples [9] = float[9](
	texture(realDepth, coords + vec2(-offsetX,offsetY)).r,
	texture(realDepth, coords + vec2(-offsetX,0)).r,
	texture(realDepth, coords + vec2(-offsetX,-offsetY)).r,
	texture(realDepth, coords + vec2(0,offsetY)).r,
	thisDepth,
	texture(realDepth, coords + vec2(0,-offsetY)).r,
	texture(realDepth, coords + vec2(offsetX,offsetY)).r,
	texture(realDepth, coords + vec2(offsetX,0)).r,
	texture(realDepth, coords + vec2(offsetX,-offsetY)).r);
	
	for(int i = 0; i < 9; i++) {
		
		if(samples[i] == -1) {
			result += thisDepth / 9.0;
		}
		else {
			result += samples[i] / 9.0;
		}
	}	


	
	return vec4(result, 0,0,1);
	
}

vec4 lowPassFilter5x5(vec2 coords) {
	float thisDepth = texture(realDepth, coords).r;
	if(thisDepth == -1.0) {
		return vec4(0,1,0,1);
	}
	float result = 0.0;
	
	for(int i = -2; i <= 2; i++) {
		for(int j = -2; j <= 2; j++) {
			
			float d = texture(realDepth, coords + vec2(i * offsetX, j * offsetY)).r;
			if(d == -1) {
				result += thisDepth / 25.0;
			}
			else {
				result += d / 25.0;
			}
			
			
			
		}	
	}	
	
	return vec4(result, 0,0,1);
	
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
	//colorOut = vec4(texture(realDepth, v_texcoord).r,0,0,1);
	//colorOut = lowPassFilter3x3(v_texcoord);
	//colorOut = lowPassFilter5x5(v_texcoord);
	colorOut = medianFilter5x5(v_texcoord);
	
	
	
	
}