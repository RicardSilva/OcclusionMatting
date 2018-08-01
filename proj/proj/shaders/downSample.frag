#version 330


in vec2 texC;
in vec2 texCs[9];

layout(location = 0) out vec4 colorOut;



uniform sampler2D inputTexture;

uniform int textureWidth;
uniform int textureHeight;
 const float weights[9] = float[9] (
		0.0625,
		0.125,
		0.0625,
		0.125,
		0.25,
		0.125,
		0.0625,
		0.125,
		0.0625);

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

vec4 gaussianBlur3x3() {
	float offsetX = 1 / textureWidth ;
	float offsetY = 1 / textureHeight ;
	
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
	
	
	
	
	vec4 result = vec4(0,0,0,0);
    float finalAlpha = 0.0; 
	vec4 fetchedColor;
	float scaleWithAlpha;
    for(int i=0; i < 9; i++) {
        fetchedColor = samples[i];
        scaleWithAlpha = fetchedColor.a * weights[i];
        result += fetchedColor * scaleWithAlpha;
        finalAlpha += scaleWithAlpha;

    }

    if (finalAlpha <= 0.0)
        discard;
    else
    {
        result.xyz /= finalAlpha;
		result.a = finalAlpha;
		
    }
	return result;
}

vec4 gaussianBlur2x2() {
	float n = 4;
	
	float offsetX = 1 / (textureWidth * n) ;
	float offsetY = 1 / (textureHeight * n) ;
	
    vec4 p0q0 = texture2D(inputTexture, texC);
    vec4 p1q0 = texture2D(inputTexture, texC + vec2(offsetX, 0));

    vec4 p0q1 = texture2D(inputTexture, texC + vec2(0, offsetY));
    vec4 p1q1 = texture2D(inputTexture, texC + vec2(offsetX , offsetY));

    float a = fract( texC.x * (textureWidth * n)); // Get Interpolation factor for X direction.
					// Fraction near to valid data.

    vec4 pInterp_q0 = mix( p0q0, p1q0, a ); // Interpolates top row in X direction.
    vec4 pInterp_q1 = mix( p0q1, p1q1, a ); // Interpolates bottom row in X direction.

    float b = fract( texC.y * (textureHeight * n));// Get Interpolation factor for Y direction.
    return mix( pInterp_q0, pInterp_q1, b ); // Interpolate in Y direction.

}


void main() {
	
		
	colorOut = gaussianBlur3x3();
	colorOut = texture(inputTexture, texC);
	
}