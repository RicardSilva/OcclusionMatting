#version 420


in vec2 texC;

layout(location = 0) out vec4 colorOut;



uniform sampler2D inputTexture;
uniform layout(rgba32f) readonly image2D inputImage;
uniform layout(rgba32f) writeonly image2D outputImage;

uniform float textureWidth;
uniform float textureHeight;



vec4 gaussianBlur3x3() {
	
	float offsetX = 1.0 / textureWidth;
	float offsetY = 1.0 / textureHeight;
	
	//imageLoad(inputTexture, ivec2(float(uWidth) * texC.x, float(uHeight) * texC.y)
	
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
	
	vec4 color = imageLoad(inputImage, ivec2(float(textureWidth) * 2 * texC.s, float(textureHeight) * 2 * texC.t));
	color = texture(inputTexture, texC);
	imageStore(outputImage,  ivec2(float(textureWidth) * texC.s, float(textureHeight) * texC.t),  color);
	discard;
	
	
	
}