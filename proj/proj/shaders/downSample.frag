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


	
	return result;
	
}




void main() {
		
	colorOut = gaussianBlur3x3();
	
	
	
}