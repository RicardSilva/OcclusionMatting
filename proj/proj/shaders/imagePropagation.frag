#version 330


in vec2 texC;

layout(location = 0) out vec4 colorOut;


uniform sampler2D finalTrimap;

uniform sampler2D inputTexture; // INPUT TEXTURE
uniform int mode; //0 -> foreground
				  //1 -> background




void main() {
			
	vec4 trimapColor = texture(finalTrimap, texC);
	if(mode == 0) {
		if(trimapColor == vec4(1,1,1,1)) { //WHITE ->	FOREGROUND
			colorOut = texture(inputTexture, texC);
		}
		else {
			colorOut = vec4(0,0,0,0);
		}
	}
	else if (mode == 1) {
		if(trimapColor == vec4(0,0,0,1)) { //BLACK ->	BACKGROUND
			colorOut = texture(inputTexture, texC);
		}
		else {
			colorOut = vec4(0,0,0,0);
		}
			
	}
	
}
