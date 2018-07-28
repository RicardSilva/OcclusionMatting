#version 330


in vec2 texC;

layout(location = 0) out vec4 colorOut;



uniform sampler2D inputTexture;

uniform float textureWidth;
uniform float textureHeight;


vec4 gaussianBlur3x3() {
	float offsetX = 1.0 / textureWidth;
	float offsetY = 1.0 / textureHeight;
	
	vec4 thisColor = texture(inputTexture, texC);
	vec4 result = vec4(0,0,0,0);
	vec4 samples [9] = vec4[9](
	texture(inputTexture, texC + vec2(-offsetX,offsetY)),
	texture(inputTexture, texC + vec2(-offsetX,0)),
	texture(inputTexture, texC + vec2(-offsetX,-offsetY)),
	texture(inputTexture, texC + vec2(0,offsetY)),
    thisColor,
	texture(inputTexture, texC + vec2(0,-offsetY)),
	texture(inputTexture, texC + vec2(offsetX,offsetY)),
	texture(inputTexture, texC + vec2(offsetX,0)),
	texture(inputTexture, texC + vec2(offsetX,-offsetY)));
	
	
	float weights[9] = float[9] (
		1,
		2,
		1,
		2,
		4,
		2,
		1,
		2,
		1);
	
	for(int i = 0; i < 9; i++) {
		
		
		if(samples[i].a < 0.9) {
			result += thisColor * weights[i] / 16.0;
		}
		else {
			result += samples[i] * weights[i] / 16.0;
		}
		
		
	}	


	if(result.a > 0) result.a = 1;
	return result;
	
}


vec4 gaussianBlur5x5() {
	
	float offsetX = 1.0 / textureWidth;
	float offsetY = 1.0 / textureHeight;
	
	vec4 thisColor = texture(inputTexture, texC);
	vec4 result = vec4(0,0,0,0);
	
	
	float weights[25] = float[25] (1, 4, 6, 4, 1,
								   4, 16, 24, 16, 4,
								   6, 24, 36, 24, 6,
								   4, 16, 24, 16, 4,
								   1, 4, 6, 4, 1);
	int counter = 0;
	for(int i = -2; i <= 2; i++) {
		for(int j = -2; j <= 2; j++) {
		
			vec4 color = texture(inputTexture, texC + vec2(i * offsetX, j * offsetY));
			if(color.a == 0) color = thisColor;
			result = result + color * weights[counter] / 256;
			counter++;
		}		
	}	


	if(result.a > 0) result.a = 1;
	return result;
	
}

vec4 gaussianBlur3x32() {
	float offsetX = 1.0 / textureWidth;
	float offsetY = 1.0 / textureHeight;
	
	vec4 thisColor = texture(inputTexture, texC);
	
	vec4 samples [9] = vec4[9](
	texture(inputTexture, texC + vec2(-offsetX,offsetY)),
	texture(inputTexture, texC + vec2(-offsetX,0)),
	texture(inputTexture, texC + vec2(-offsetX,-offsetY)),
	texture(inputTexture, texC + vec2(0,offsetY)),
    thisColor,
	texture(inputTexture, texC + vec2(0,-offsetY)),
	texture(inputTexture, texC + vec2(offsetX,offsetY)),
	texture(inputTexture, texC + vec2(offsetX,0)),
	texture(inputTexture, texC + vec2(offsetX,-offsetY)));
	
	
	float weights[9] = float[9] (
		1/16.0,
		2/16.0,
		1/16.0,
		2/16.0,
		4/16.0,
		2/16.0,
		1/16.0,
		2/16.0,
		1/16.0);
	
	vec4 result = vec4(0,0,0,0);
    float finalAlpha = 0.0; // The actual end alpha

    for(int i=0; i < 9; i++) {
        vec4 fetchedColor = samples[i];
        float scaleWithAlpha = fetchedColor.a * weights[i];
        result += fetchedColor*scaleWithAlpha;
        finalAlpha += scaleWithAlpha;

    }

    if (finalAlpha <= 0.0)
        discard;
    else
    {
        result /= finalAlpha;
		result.a = finalAlpha;
		
    }
	return result;
}

void main() {
	
		
	colorOut = gaussianBlur3x32();
	
	//colorOut = gaussianBlur5x5();
	
}