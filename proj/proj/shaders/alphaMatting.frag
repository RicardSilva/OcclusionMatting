#version 330


in vec2 texC;

layout(location = 0) out vec4 colorOut;


uniform sampler2D virtualColor;
uniform sampler2D virtualDepth;
uniform sampler2D realColor;
uniform sampler2D realDepth;

const float zNear = 0.1;
const float zFar = 8000.0;



void main() {

	
	
	float z_b = texture(virtualDepth, texC).r;	 
	float depthV = (2.0 * zNear) / (zFar + zNear - z_b * (zFar - zNear));
	vec4 colorV = texture(virtualColor, texC);
	
	vec2 v_texcoord = vec2(texC.s, 1.0 - texC.t);
	float depthR = texture(realDepth, v_texcoord).r;	
	vec4 colorR = texture(realColor, v_texcoord);

	if(depthR == 1 && depthV < 0.9)
		colorOut = colorV;
	else if(depthR == -1 && depthV < 0.9)
		colorOut = colorR;
	else if(depthR < depthV || depthV > 0.9 )
		colorOut = colorR;
	else
		colorOut = colorV;
	
	//colorOut = vec4(depthV, depthV, depthV, 1.0);
	//colorOut = colorV;
	//colorOut = vec4(depthR, depthR, depthR, 1.0);
	//colorOut = mix(colorOut, vec4(depthR, depthR, depthR, 1.0), 0.5);
	//colorOut = colorR;
	//colorOut = mix(vec4(depthR, depthR, depthR, 1.0), colorR, 0.5);
	//if ( z_b < 1 )
	//	colorOut = vec4(1.0,0.0,0.0,1.0);
	//else
	//	colorOut = vec4(0.0,1.0,0.0,1.0);
	
	//colorOut = vec4(z_b, z_b, z_b, 1.0);
	
	
	//if(texC.s < 0.5)
	//	colorOut = vec4(depthR, depthR, depthR, 1.0);
	//else
	//	colorOut = vec4(depthV, depthV, depthV, 1.0);
	
	
	
	
	
	
	
	
	
	
}