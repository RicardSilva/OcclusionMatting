#version 330


in vec2 texC;

layout(location = 0) out vec4 colorOut;


uniform sampler2D virtualColor;
uniform sampler2D realColor;

uniform sampler2D alphaMap;
uniform sampler2D alphaMap2;
uniform sampler2D alphaMap3;
uniform sampler2D alphaMap4;
uniform sampler2D alphaMap5;
uniform sampler2D alphaMap6;
uniform sampler2D alphaMap7;
uniform sampler2D alphaMap8;
uniform sampler2D alphaMap9;
uniform sampler2D alphaMap10;


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

void main() {
	alphaValues[0] = texture(alphaMap, texC).r;
	alphaValues[1] = texture(alphaMap2, texC).r;
	alphaValues[2] = texture(alphaMap3, texC).r;
	alphaValues[3] = texture(alphaMap4, texC).r;
    alphaValues[4] = texture(alphaMap5, texC).r;
	
	float finalAlpha = median();
	//float finalAlpha = alphaValues[0];
	colorOut = finalAlpha * texture(realColor , texC)  + (1 - finalAlpha) * texture(virtualColor, texC);
	
}


