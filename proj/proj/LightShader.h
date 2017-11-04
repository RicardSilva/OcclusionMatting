#pragma once
#include <GL/glew.h>
#include "Shader.h"
#include <map>
#include <string>
#include "AVTmathLib.h"
#include <sstream>
#include "Light.h"

#define VERTICES 0
#define NORMALS 1
#define TEXCOORDS 2

#define MAX_LIGHTS 9
#define ATTRIBS_PER_LIGHT 11

// The storage for matrices
extern float mMatrix[COUNT_MATRICES][16];
extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];

// The normal matrix
extern float mNormal3x3[9];

class LightShader : public Shader {

private:

	GLint projViewModelID;
	GLint viewModelID;
	GLint normalID;
	
	GLint matAmbientID;
	GLint matDiffuseID;
	GLint matSpecularID;
	GLint matShininessID;
	GLint matTransparencyID;

	GLint useTexturesID;
	GLint woodDiffuseID;
	GLint woodSpecularID;
	GLint bambooDiffuseID;
	GLint bambooSpecularID;
	GLint maskID;

	GLint lightUniforms[MAX_LIGHTS][ATTRIBS_PER_LIGHT];
	GLchar* lightAttribNames[ATTRIBS_PER_LIGHT] = { "isActive", "type", "position", "direction",
		"color", "intensity", "constantAttenuation", "linearAttenuation", "quadraticAttenuation", "spotCosCutoff", "spotExponent" };


public:
	LightShader(const std::string& vertexShader, const std::string& fragmentShader) : Shader(vertexShader, fragmentShader) {
		bindAttributes();
		getUniformLocations();
	}
	virtual ~LightShader() {
	}

	void bindAttributes(void) {
		bindAttribute(VERTICES, "position");
		bindAttribute(NORMALS, "normal");
		bindAttribute(TEXCOORDS, "texCoord");
	}
	void getUniformLocations(void) {
		projViewModelID = getUniformLocation("m_pvm");
		viewModelID = getUniformLocation("m_viewModel");
		normalID = getUniformLocation("m_normal");

		matAmbientID = getUniformLocation("mat.ambient");
		matDiffuseID = getUniformLocation("mat.diffuse");
		matSpecularID = getUniformLocation("mat.specular");
		matShininessID = getUniformLocation("mat.shininess");
		matTransparencyID = getUniformLocation("mat.transparency");
		
		useTexturesID = getUniformLocation("useTextures");
		woodDiffuseID = getUniformLocation("woodDiffuse");
		woodSpecularID = getUniformLocation("woodSpecular");
		bambooDiffuseID = getUniformLocation("bambooDiffuse");
		bambooSpecularID = getUniformLocation("bambooSpecular");
		maskID = getUniformLocation("mask");

		std::string uniformName;
		for (int i = 0; i < MAX_LIGHTS; i++) {
			for (int j = 0; j < ATTRIBS_PER_LIGHT; j++) {
				uniformName = "lights[" + std::to_string(i) + "]." + lightAttribNames[j];
				lightUniforms[i][j] = getUniformLocation(uniformName.c_str());
			}
			

		}
	}
		
	void loadProjViewModelMatrix(float* matrix) {
		Shader::loadMat4(projViewModelID, matrix);
	}
	void loadViewModelMatrix(float* matrix) {
		Shader::loadMat4(viewModelID, matrix);
	}
	void loadNormalMatrix(float* matrix) {
		Shader::loadMat3(normalID, matrix);
	}
	void loadMaterial(Material& material) {
		Shader::loadVec3(matAmbientID, material.Ka);
		Shader::loadVec3(matDiffuseID, material.Kd);
		Shader::loadVec3(matSpecularID, material.Ks);
		Shader::loadFloat(matShininessID, material.Ns);
		Shader::loadFloat(matTransparencyID, material.d);
	}
	void loadDirectionalLight(Light& light) {
		vec4 lightDir = multMatrixPoint(VIEW, light.direction);
		int lightId = light.id;
		Shader::loadBool(lightUniforms[lightId][0], light.isActive);
		Shader::loadInt(lightUniforms[lightId][1], light.type);
		Shader::loadVec4(lightUniforms[lightId][3], lightDir);
		Shader::loadVec3(lightUniforms[lightId][4], light.color);
		Shader::loadFloat(lightUniforms[lightId][5], light.intensity);
	}
	void subLoadDirectionalLight(Light& light) {
		vec4 lightDir = multMatrixPoint(VIEW, light.direction);
		int lightId = light.id;

		Shader::loadBool(lightUniforms[lightId][0], light.isActive);
		Shader::loadVec4(lightUniforms[lightId][3], lightDir);
	}
	void loadPointLight(Light& light) {
		vec4 lightPos = multMatrixPoint(VIEW, light.position);
		int lightId = light.id;

		Shader::loadBool(lightUniforms[lightId][0], light.isActive);
		Shader::loadInt(lightUniforms[lightId][1], light.type);
		Shader::loadVec4(lightUniforms[lightId][2], lightPos);
		Shader::loadVec3(lightUniforms[lightId][4], light.color);
		Shader::loadFloat(lightUniforms[lightId][5], light.intensity);

		Shader::loadFloat(lightUniforms[lightId][6], light.constantAttenuation);
		Shader::loadFloat(lightUniforms[lightId][7], light.linearAttenuation);
		Shader::loadFloat(lightUniforms[lightId][8], light.quadraticAttenuation);

	}
	void subLoadPointLight(Light& light) {
		vec4 lightPos = multMatrixPoint(VIEW, light.position);
		int lightId = light.id;

		Shader::loadBool(lightUniforms[lightId][0], light.isActive);
		Shader::loadVec4(lightUniforms[lightId][2], lightPos);
	}
	void loadSpotLight(Light& light) {
		vec4 lightPos = multMatrixPoint(VIEW, light.position);
		vec4 lightDir = multMatrixPoint(VIEW, light.direction);
		int lightId = light.id;

		Shader::loadBool(lightUniforms[lightId][0], light.isActive);
		Shader::loadInt(lightUniforms[lightId][1], light.type);
		Shader::loadVec4(lightUniforms[lightId][2], lightPos);
		Shader::loadVec4(lightUniforms[lightId][3], lightDir);
		Shader::loadVec3(lightUniforms[lightId][4], light.color);
		Shader::loadFloat(lightUniforms[lightId][5], light.intensity);

		Shader::loadFloat(lightUniforms[lightId][6], light.constantAttenuation);
		Shader::loadFloat(lightUniforms[lightId][7], light.linearAttenuation);
		Shader::loadFloat(lightUniforms[lightId][8], light.quadraticAttenuation);

		Shader::loadFloat(lightUniforms[lightId][9], light.spotCosCutoff);
		Shader::loadFloat(lightUniforms[lightId][10], light.spotExponent);

	}
	void subLoadSpotLight(Light& light) {
		vec4 lightPos = multMatrixPoint(VIEW, light.position);
		vec4 lightDir = multMatrixPoint(VIEW, light.direction);
		int lightId = light.id;

		Shader::loadBool(lightUniforms[lightId][0], light.isActive);
		Shader::loadVec4(lightUniforms[lightId][2], lightPos);
		Shader::loadVec4(lightUniforms[lightId][3], lightDir);
	}
	void enableTextures() {
		Shader::loadBool(useTexturesID, true);
	}
	void disableTextures() {
		Shader::loadBool(useTexturesID, false);
	}
	void loadWoodDiffuse(GLint id) {
		Shader::loadInt(woodDiffuseID, id);
	}
	void loadWoodSpecular(GLint id) {
		Shader::loadInt(woodSpecularID, id);
	}
	void loadBambooDiffuse(GLint id) {
		Shader::loadInt(bambooDiffuseID, id);
	}
	void loadBambooSpecular(GLint id) {
		Shader::loadInt(bambooSpecularID, id);
	}
	void loadMask(GLint id) {
		Shader::loadInt(maskID, id);
	}

	void loadMatrices() {
		computeDerivedMatrix(PROJ_VIEW_MODEL);
		loadViewModelMatrix(mCompMatrix[VIEW_MODEL]);
		loadProjViewModelMatrix(mCompMatrix[PROJ_VIEW_MODEL]);
		computeNormalMatrix3x3();
		loadNormalMatrix(mNormal3x3);
	}


};



