#version 330


in Data {
	vec4 pos; 
	vec3 normal;
	vec3 eye;
	vec2 texCoord;
} DataIn;

out vec4 colorOut;

struct Material {
	vec3 diffuse;
	vec3 ambient;
	vec3 specular;
	vec3 emissive;
	float shininess;
	float transparency;
	int texCount;
};

struct Light {
	bool isActive;
	int type;
	vec4 position;
	vec4 direction;
	vec3 color;	
	float intensity;
	
	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
	
	float spotCosCutoff; 
	float spotExponent;
}; 

// the set of lights to apply, per invocation of this shader
const int MaxLights = 9;
uniform Light lights[MaxLights];

uniform bool useTextures;
uniform Material mat;

uniform sampler2D woodDiffuse;
uniform sampler2D woodSpecular;
uniform sampler2D bambooDiffuse;
uniform sampler2D bambooSpecular;
uniform sampler2D mask;



vec4 calcDirLight(Light light, vec3 normal, vec3 viewDir, vec3 materialDiffuse, vec3 materialSpecular) {

	vec4 lightDir4 = normalize(-light.direction);
	vec3 lightDir = vec3(lightDir4);
	float diff = max(0.0, dot(normal, lightDir));
	
	vec3 HalfVector = normalize(lightDir + viewDir);
	float spec = max(0.0, dot(normal, HalfVector));
	
	// surfaces facing away from the light (negative dot products)
	// won’t be lit by the directional light
	if (diff == 0.0)
		spec = 0.0;
	else
		spec = pow(spec, mat.shininess); // sharpen the highlight
		
	vec3 diffuse = light.color * diff * materialDiffuse;
	vec3 specular = light.color * spec * materialSpecular;
	return vec4((diffuse + specular).xyz, 1.0);
}

vec4 calcPointLight(Light light, vec4 position, vec3 normal, vec3 viewDir, vec3 materialDiffuse, vec3 materialSpecular) {

	vec4 lightDirection4 = light.position - position;
	vec3 lightDirection = vec3(lightDirection4);
	float lightDistance = length(lightDirection);
	
	// normalize the light direction vector, so
	// that a dot products give cosines
	lightDirection = lightDirection / lightDistance;
	
	// model how much light is available for this fragment
	float attenuation = 1.0 / (light.constantAttenuation 
	+ light.linearAttenuation * lightDistance 
	+ light.quadraticAttenuation * lightDistance * lightDistance);
	
	// the direction of maximum highlight also changes per fragment
	vec3 halfVector = normalize(lightDirection + viewDir);
	
	float diff = max(0.0, dot(normal, lightDirection));
	float spec = max(0.0, dot(normal, halfVector));
	
	if (diff == 0.0)
		spec = 0.0;
	else
		spec = pow(spec, mat.shininess);
				
	vec3 diffuse = light.color * diff * materialDiffuse * attenuation;
	vec3 specular = light.color * spec * materialSpecular * attenuation;
	return vec4((diffuse + specular ).xyz, 1.0);

}

vec4 calcSpotLight(Light light, vec4 position, vec3 normal, vec3 viewDir, vec3 materialDiffuse, vec3 materialSpecular) {
	
	vec4 lightDirection4 = light.position - position;
	vec3 lightDirection = vec3(lightDirection4);
	float lightDistance = length(lightDirection);
	lightDirection = lightDirection / lightDistance;
	
	float attenuation = 1.0 / (light.constantAttenuation 
	+ light.linearAttenuation * lightDistance 
	+ light.quadraticAttenuation * lightDistance * lightDistance);
	
	// how close are we to being in the spot?
	float spotCos = dot(lightDirection, (vec3(-light.direction)));
	// attenuate more, based on spot-relative position
	if (spotCos < light.spotCosCutoff)
		attenuation = 0.0;
	else
		attenuation *= pow(spotCos, light.spotExponent);
	
	vec3 halfVector = normalize(lightDirection + viewDir);
	
	float diff = max(0.0, dot(normal, lightDirection));
	float spec = max(0.0, dot(normal, halfVector));
	if (diff == 0.0)
		spec = 0.0;
	else
		spec = pow(spec, mat.shininess);
		
	vec3 diffuse = light.color * diff * materialDiffuse * attenuation;
	vec3 specular = light.color * spec * materialSpecular * attenuation;
	
	return vec4((diffuse + specular).xyz, 1.0);

}

void main() {

	colorOut = vec4(0);
	
	
	vec3 materialDiffuse;
	vec3 materialSpecular;
	float materialTransparency;
	vec3 normal;
	if(useTextures) {
		vec3 woodDiff = vec3(texture(woodDiffuse, DataIn.texCoord * 15));
		vec3 bambooDiff = vec3(texture(bambooDiffuse, DataIn.texCoord * 10));
		float mixCoefficient = (texture(mask, DataIn.texCoord)).r;
		materialDiffuse = mix(woodDiff, bambooDiff, mixCoefficient);
		colorOut += vec4(materialDiffuse * 0.1, 1);	
		
		
		vec3 woodSpec = vec3(texture(woodSpecular, DataIn.texCoord * 15));
		vec3 bambooSpec = vec3(texture(bambooSpecular, DataIn.texCoord * 10)) * 0.9;
		materialSpecular = mix(woodSpec, bambooSpec, mixCoefficient) ;
		materialTransparency = 1.0f;
		
		
	}
	else {
		colorOut += vec4(mat.ambient.xyz, 1);	
		materialDiffuse = mat.diffuse.xyz;
		materialSpecular = mat.specular.xyz;
		materialTransparency = mat.transparency;
	}
	
	normal = normalize(DataIn.normal);
	
	for (int i = 0; i < MaxLights; i++) {
		Light light = lights[i];
		if(light.isActive) {
			
			if (light.type == 1)
				colorOut += calcPointLight(light, DataIn.pos, normal, DataIn.eye, materialDiffuse, materialSpecular) * light.intensity;
			else if (light.type == 2)
				colorOut += calcSpotLight(light, DataIn.pos, normal, DataIn.eye, materialDiffuse, materialSpecular) * light.intensity;
			else if(light.type == 0)	
				colorOut += calcDirLight(light, normal, DataIn.eye, materialDiffuse, materialSpecular) * light.intensity;
			
		}
	}
	
	colorOut.w = materialTransparency;
	
	
	
}