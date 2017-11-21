#version 330


in vec2 texC;

layout(location = 0) out vec4 colorOut;


uniform sampler2D virtualColor;
uniform sampler2D virtualDepth;
uniform sampler2D realColor;
uniform sampler2D realDepth;




void main() {

	
	/*
	float z = texture(virtualDepth, texC).r;      // fetch the z-value from our depth texture
	float n = 1.0;                                // the near plane
	float f = 30.0;                               // the far plane
	float c = (2.0 * n) / (f + n - z * (f - n));  // convert to linear values 
	 
	colorOut = vec4(c,c,c,1);                      // linear
	*/
	vec2 v_texcoord = vec2(texC.s, 1.0 - texC.t);
	colorOut = texture(realDepth, v_texcoord);
		
	
}