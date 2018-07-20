#version 330


in vec2 texC;

layout(location = 0) out vec4 colorOut;


uniform sampler2D inputTexture; // INPUT TEXTURE F
uniform sampler2D inputTexture2; // INPUT TEXTURE S
uniform int iteration;

const float n = 0.5;


void main() {
	
	vec4 sampleF = texture(inputTexture, texC);
	vec4 sampleS = texture(inputTexture2, texC);
	if(	sampleS.a > 0) {
	
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
