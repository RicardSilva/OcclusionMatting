#version 420


in vec2 texC;

layout(location = 0) out vec4 colorOut;


uniform sampler2D inputTexture; // INPUT TEXTURE
uniform layout(rgba32f) writeonly image2D outputImage;




void main() {
		
	vec4 color = texture(inputTexture, texC);
	if(	color.a > 0) {
		color.a = 1;
		imageStore(outputImage, ivec2(gl_FragCoord.xy) , color);
	}
	else {
		color.a = 0;
		imageStore(outputImage, ivec2(gl_FragCoord.xy) , color);
	}
	discard;
}
