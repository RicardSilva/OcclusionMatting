#version 330


in vec2 texC;

layout(location = 0) out vec4 colorOut;


uniform sampler2D inputTexture; // INPUT TEXTURE F
uniform sampler2D inputTexture2; // INPUT TEXTURE S
uniform int mode; //0 -> foreground
				  //1 -> background

const float n 0.5;


void main() {
		
	vec4 sampleS = texture(inputTexture2, texC);
	vec4 sampleF = texture(inputTexture, texC);
	if(	sampleS.a > 0) {
	
		float w = sampleF.a / n;
	
		float a  = min(sampleF.a + 1, n);
	
		vec4 color = w * sampleF + (1-w) * sampleS;
		color.a = alpha;
		colorOut = color;
	}
	else {
		colorOut = sample;
	}
}
