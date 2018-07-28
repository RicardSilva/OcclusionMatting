#version 420

uniform layout(rgba32f) readonly image2D inputImage;
uniform layout(rgba32f) writeonly image2D outputImage;

uniform int textureWidth;
uniform int textureHeight;

// sample with clamping from the texture. 
vec4 csample(ivec2 i) {
  i = ivec2(clamp(i.x, 0, textureWidth-1), clamp(i.y, 0, textureHeight-1));
  return imageLoad(inputImage, i);
}

#define R 8// filter radius
#define W (1.0 / ((1.0+2.0*float(R)) * (1.0+2.0*float(R)))) // this macro computes the filter weights. 
void main() {
	ivec2 i = ivec2(gl_VertexID % textureWidth, gl_VertexID / textureWidth);
	vec4 sum = vec4(0.0);
	// first compute the blurred color. 
	for(int x = -R; x <= +R; x++ )
		for(int y = -R; y <= +R; y++ )
			sum += W * csample(i + ivec2(x,y));

	// now store the blurred color.
	imageStore(outputImage,  i, vec4(1,0,0,1));

}