#version 420


in vec2 texC;

layout(location = 0) out vec4 colorOut;


uniform sampler2D inputTexture; // INPUT TEXTURE F
uniform sampler2D inputTexture2; // INPUT TEXTURE S

uniform layout(rgba32f) writeonly image2D outputImage;
uniform layout(rgba32f) writeonly image2D outputImage2;
uniform int iteration;

const float n = 0.8;


void main() {
	
	vec4 sampleF = texture(inputTexture, texC);
	
	vec4 sampleS = texture(inputTexture2, texC);
	
	vec4 result;
	if(	sampleS.a > 0) {
		//if(sampleF.a == 0) {
			//save iteration for propagation distance metric
		//}
	
		float w = sampleF.a / n;
		
		//vec4 color = w * sampleF + (1-w) * sampleS;
		vec4 color;
		color.xyz = mix(sampleS.xyz, sampleF.xyz, w);
		//color.xyz = w * sampleF.xyz + (1-w) * sampleS.xyz;
		color.a = min(sampleF.a + 1, n);
		result = color;
	}
	else {
		result = sampleF;
	}
	
	//result = texture(inputTexture2, texC);
	imageStore(outputImage, ivec2(gl_FragCoord.xy) , result);
	discard;
	
	
}