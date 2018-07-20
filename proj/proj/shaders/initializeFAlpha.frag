#version 330


in vec2 texC;

layout(location = 0) out vec4 colorOut;


uniform sampler2D inputTexture; // INPUT TEXTURE




void main() {
		
	colorOut = texture(inputTexture, texC);
	colorOut.a = 0.5f;
}
