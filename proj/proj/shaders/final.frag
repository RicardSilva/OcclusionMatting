#version 330


in vec2 texC;

layout(location = 0) out vec4 colorOut;


uniform sampler2D virtualColor;
uniform sampler2D realColor;

uniform sampler2D alphaMap;
uniform sampler2D alphaMap2;

uniform sampler2D realSmoothDepth;


const float resX = 1920.0;
const float resY = 1080.0;

const float offsetX = 1.0 / resX;
const float offsetY = 1.0 / resY;

float alphaValues[5];

void bubbleSort(int size) {
	int items_to_sort = size / 2 + 1;
    for(int k = 0; k < items_to_sort; k++) {
        for(int j = 0; j < size - 1 - k; j++) {
            if(alphaValues[j] > alphaValues[j + 1]) {
                float tmp = alphaValues[j];
                alphaValues[j] = alphaValues[j+1];
                alphaValues[j+1] = tmp;
                
            }           
                        
        }
    }
}

float median() {
	bubbleSort(5);
	return alphaValues[2];


}

float average() {
	float sum = 0;
	for(int i = 0; i < 5; i++)
		sum += alphaValues[i];

	return sum / 10.0f;
}

float lowPassFilter5x5(vec2 coords, sampler2D sampler) {
	float thisAlpha = texture(sampler, coords).r;
	float result = 0.0;
	int size = 2;
	if(thisAlpha == 0 || thisAlpha == 1) return thisAlpha;
	for(int i = -size; i <= size; i++) {
		for(int j = -size; j <= size; j++) {
			
			float d = texture(sampler, coords + vec2(i * offsetX, j * offsetY)).r;
			if(d == 0)
				result += (thisAlpha - 0.2) / 25.0 ;
			else
				result += d / 25.0;
			
			
			
			
		}	
	}	
	
	return result;
	
}

float lowPassFilterRough5x5(vec2 coords, sampler2D sampler) {
	float thisAlpha = texture(sampler, coords).r;
	float result = 0.0;
	int size = 2;
	for(int i = -size; i <= size; i++) {
		for(int j = -size; j <= size; j++) {
			
			float d = texture(sampler, coords + vec2(i * offsetX, j * offsetY)).r;
			result += d / 25.0;
			
			
			
			
		}	
	}	
	
	return result;
	
}

void main() {
	alphaValues[0] = texture(alphaMap, texC).r;
	
	//float finalAlpha = median();
	//float finalAlpha = alphaValues[0];
	float finalAlpha = lowPassFilterRough5x5(texC, alphaMap);
	
	colorOut = finalAlpha * texture(realColor , texC)  + (1 - finalAlpha) * texture(virtualColor, texC);
	colorOut = vec4(finalAlpha, finalAlpha,finalAlpha, 1);
	//colorOut = texture(realSmoothDepth, texC) * 2;
}


