#version 420


in vec2 texC;

layout(location = 0) out vec4 colorOut;


uniform sampler2D inputTexture; // INPUT TEXTURE F
uniform sampler2D inputTexture2; // INPUT TEXTURE S

uniform layout(rgba32f) writeonly image2D outputImage;
uniform layout(rgba32f) image2D outputImage2;
uniform float iteration;

const float n = 0.5;
const float iterations = 5;

void main() {
	
	vec4 sampleF = texture(inputTexture, texC);
	
	vec4 sampleS = texture(inputTexture2, texC);
	vec4 result;
	if(	sampleS.a > 0) {
		if(imageLoad(outputImage2, ivec2(gl_FragCoord.xy)) == vec4(0,0,0,0)) {
			imageStore(outputImage2, ivec2(gl_FragCoord.xy) , vec4(iteration*0.1, 0,0, 1));
		}
		
	
		float w = sampleF.a / n;
		
		vec4 color;
		//color.xyz = mix(sampleS.xyz, sampleF.xyz, w);
		color.xyz = w * sampleF.xyz + (1-w) * sampleS.xyz;
		
		color.a = min(sampleF.a + 1, n);
		result = color;
	}
	else {
		result = sampleF;
	}
	if(iteration == iterations - 1) {
		result.a = 1;
	}
	imageStore(outputImage, ivec2(gl_FragCoord.xy) , result);
	//colorOut = result;
	discard;

	
	
	
}
