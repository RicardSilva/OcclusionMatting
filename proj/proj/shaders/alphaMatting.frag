#version 330


in vec2 texC;

layout(location = 0) out vec4 colorOut;


uniform sampler2D virtualColor;
uniform sampler2D virtualDepth;
uniform sampler2D realColor;
uniform sampler2D realDepth;

const float zNear = 0.1;
const float zFar = 30.0;



void main() {

	
	
	float z_b = texture(virtualDepth, texC).r;
    float z_n = 2.0 * z_b - 1.0;
    float depthV = 2.0 * zNear * zFar / (zFar + zNear - z_n * (zFar - zNear)) * 100;	 
	vec4 colorV = texture(virtualColor, texC);
	
	vec2 v_texcoord = vec2(texC.s, 1.0 - texC.t);
	vec4 depthR = texture(realDepth, v_texcoord);	
	vec4 colorR = texture(realColor, v_texcoord);
	
	if(depthV < depthR.r || depthV == depthR.r)
		colorOut = colorV;
	else
		colorOut = colorR;
	
	//colorOut = vec4(depthV, depthV, depthV, 1.0);
	//colorOut = vec4(depthR.r, depthR.r, depthR.r, 1.0);
	//colorOut = colorR;
	//colorOut = mix(depthR.r, colorR, 0.5);
	//if ( depthR.r == 0 )
	//	colorOut = vec4(1.0,0.0,0.0,1.0);
	//else
	//	colorOut = vec4(0.0,1.0,0.0,1.0);
	
}