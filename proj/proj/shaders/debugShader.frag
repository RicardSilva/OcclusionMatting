#version 420

in vec2 texC;

layout(location = 0) out vec4 colorOut;


uniform sampler2D inputTexture;
uniform layout(rgba32f) readonly image2D inputImage;



void main() {
			
	
	vec4 c = texture(inputTexture, texC);
	
	//colorOut = imageLoad(inputImage, ivec2(gl_FragCoord.xy));
	
	colorOut = c;
	//if(colorOut.a < 1 && colorOut.a > 0) colorOut = vec4(1,0,0,1);
	//if(colorOut.a == 1) colorOut = vec4(0,1,0,1);
	
	
}
