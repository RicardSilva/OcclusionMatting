#version 420

in vec2 texC;

layout(location = 0) out vec4 colorOut;


uniform sampler2D inputTexture;
uniform layout(rgba32f) image2D outputImage;



void main() {
			
	
	//vec4 sample = texture(inputTexture, texC);
	
	//colorOut = sample;
	
	//if(sample.r == 1) colorOut = vec4(0,1,0,1);
	colorOut = imageLoad(outputImage, ivec2(gl_FragCoord.xy));
	
}
