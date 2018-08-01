#version 420

in vec2 texC;

layout(location = 0) out vec4 colorOut;


uniform sampler2D inputTexture;



void main() {
			
	
	vec4 c = texture(inputTexture, texC);	
	//c.a = 1;
	colorOut = c;
	//if(colorOut.a < 1 && colorOut.a > 0) colorOut = vec4(1,0,0,1);
	//if(c.a  ==  0.8) colorOut = vec4(0,1,0,1);
	
	
}
