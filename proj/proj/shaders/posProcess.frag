#version 330


in vec2 texC;

layout(location = 0) out vec4 colorOut;


uniform sampler2D inputTexture; // INPUT TEXTURE F
uniform sampler2D inputTexture2; // INPUT TEXTURE S
uniform int iteration;

const float n = 0.8;


void main() {
	
	vec4 sampleF = texture(inputTexture, texC);
	float aF = sampleF.a;
	sampleF = sampleF / aF;
	sampleF.a = aF;
	vec4 sampleS = texture(inputTexture2, texC);
	float aS = sampleS.a;
	sampleS = sampleS / aS;
	sampleS.a = aS;
	if(	sampleS.a > 0.9) {
	
		float w = sampleF.a / n;
		
		vec4 color = w * sampleF + (1-w) * sampleS;
		float alpha  = min(sampleF.a + 1, n);
	
		color.a = alpha;
		colorOut = color;
	}
	else {
		colorOut = sampleF;
	}
	
}
