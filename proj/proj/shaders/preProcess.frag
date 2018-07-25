#version 330


in vec2 texC;

layout(location = 0) out vec4 colorOut;


uniform sampler2D inputTexture; // INPUT TEXTURE




void main() {
		
	vec4 sample = texture(inputTexture, texC);
	if(	sample.a > 0) {
		colorOut = sample / sample.a;
		colorOut.a = 1;
	}
	else {
		colorOut = vec4(sample.r, sample.g, sample.b, 0);
	}
}
