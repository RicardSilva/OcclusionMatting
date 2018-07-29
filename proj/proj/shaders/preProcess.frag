#version 420


in vec2 texC;



uniform sampler2D inputTexture; // INPUT TEXTURE
uniform layout(rgba16f) writeonly image2D outputImage;




void main() {
		
	vec4 color = texture(inputTexture, texC);
	if(	color.a > 0) {
		color.a = 1;
	}
	else {
		color.a = 0;
		
	}
	imageStore(outputImage, ivec2(gl_FragCoord.xy) , color);
	discard;
}
