#version 420




in vec2 texC;

layout(location = 0) out vec4 colorOut;


uniform sampler2D finalTrimap;

uniform sampler2D inputTexture; // INPUT TEXTURE
uniform layout(rgba32f) writeonly image2D outputImage;


uniform int mode; //0 -> foreground
				  //1 -> background


const float n = 0.8f;

void main(void) {
	vec4 color;
	vec4 trimapColor = texture(finalTrimap, texC);
	if(mode == 0) {
		if(trimapColor == vec4(1,1,1,1)) { //WHITE ->	FOREGROUND
			color = texture(inputTexture, texC);
			color.a = n;
		}
		else {
			color = vec4(0,0,0,0);
		}
	}
	else if (mode == 1) {
		if(trimapColor == vec4(0,0,0,1)) { //BLACK ->	BACKGROUND
			color = texture(inputTexture, texC);
			color.a = n;
		}
		else {
			color = vec4(0,0,0,0);
		}
			
	}
	
	/*if((0.5-texC.s) * (0.5-texC.s)  + (0.5-texC.t) * (0.5-texC.t)  <= 0.025) {
		color = vec4(texC.s, texC.t, 0, 1);
		color.a = 0.3f;
		
	}
	else {
		color = vec4(0,0,0,0);
	}*/
	imageStore(outputImage, ivec2(gl_FragCoord.xy) , vec4(0));
	//discard;
	colorOut = color;
	
	
	
}
