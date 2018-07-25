#version 330


in vec2 texC;

layout(location = 0) out vec4 colorOut;


uniform sampler2D inputTexture;


void main() {
	vec4 sample = texture(inputTexture, texC);
	
	colorOut = sample / sample.a;
	colorOut.a = sample.a;
}
