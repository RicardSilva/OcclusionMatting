#version 330

in vec2 texC;

layout(location = 0) out vec4 colorOut;


uniform sampler2D inputTexture;

uniform int textureWidth;
uniform int textureHeight;



float BSpline(float x) {
	float f = x;
	if( f < 0.0 )
	{
		f = -f;
	}

	if( f >= 0.0 && f <= 1.0 )
	{
		return ( 2.0 / 3.0 ) + ( 0.5 ) * ( f* f * f ) - (f*f);
	}
	else if( f > 1.0 && f <= 2.0 )
	{
		return 1.0 / 6.0 * pow( ( 2.0 - f  ), 3.0 );
	}
	return 1.0;
}  

vec4 BiCubic( sampler2D textureSampler, vec2 TexCoord) {
    float texelSizeX = 1.0 / textureWidth; //size of one texel 
    float texelSizeY = 1.0 / textureHeight; //size of one texel 
    vec4 nSum = vec4( 0.0, 0.0, 0.0, 0.0 );
    vec4 nDenom = vec4( 0.0, 0.0, 0.0, 0.0 );
    float a = fract( TexCoord.x * textureWidth ); // get the decimal part
    float b = fract( TexCoord.y * textureHeight ); // get the decimal part
    for( int m = -2; m <=2; m++ )
    {
        for( int n =-2; n<= 2; n++)
        {
			vec4 vecData = texture(textureSampler, TexCoord + vec2(texelSizeX * float( m ), texelSizeY * float( n )));
			float f  = BSpline( float( m )) * vecData.a;
			vec4 vecCooef1 = vec4( f,f,f,f ) ;
			float f1 = BSpline ( -( float( n )) * vecData.a) ;
			vec4 vecCoeef2 = vec4( f1, f1, f1, f1 );
            nSum = nSum + ( vecData * vecCoeef2 * vecCooef1  );
            nDenom = nDenom + (( vecCoeef2 * vecCooef1 ));
        }
    }
    return nSum / nDenom;
}



vec4 cubic2(float x) // cubic_catmullrom(float x)
{
    const float s = 0.5; // potentially adjustable parameter
    float x2 = x * x;
    float x3 = x2 * x;
    vec4 w;
    w.x =    -s*x3 +     2*s*x2 - s*x + 0;
    w.y = (2-s)*x3 +   (s-3)*x2       + 1;
    w.z = (s-2)*x3 + (3-2*s)*x2 + s*x + 0;
    w.w =     s*x3 -       s*x2       + 0;
    return w;
}

vec4 BiCubic2(sampler2D sampler, vec2 texCoords){

   vec2 texSize = textureSize(sampler, 0);
   vec2 invTexSize = 1.0 / texSize;

   texCoords = texCoords * texSize - 0.5;


    vec2 fxy = fract(texCoords);
    texCoords -= fxy;

    vec4 xcubic = cubic2(fxy.x);
    vec4 ycubic = cubic2(fxy.y);

    vec4 c = texCoords.xxyy + vec2 (-0.5, +1.5).xyxy;

    vec4 s = vec4(xcubic.xz + xcubic.yw, ycubic.xz + ycubic.yw);
    vec4 offset = c + vec4 (xcubic.yw, ycubic.yw) / s;

    offset *= invTexSize.xxyy;

    vec4 sample0 = texture(sampler, offset.xz);
    vec4 sample1 = texture(sampler, offset.yz);
    vec4 sample2 = texture(sampler, offset.xw);
    vec4 sample3 = texture(sampler, offset.yw);

    float sx = s.x / (s.x + s.y);
    float sy = s.z / (s.z + s.w);
	
	vec4 c1 = mix(sample3, sample2, sx);
	vec4 c2 = mix(sample1, sample0, sx);

    return mix( c1, c2, sy);
}


void main() {
	colorOut = BiCubic(inputTexture, texC);	
	//colorOut = BiCubic2(inputTexture, texC);	
}