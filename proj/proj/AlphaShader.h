#pragma once
#include <GL/glew.h>
#include "Shader.h"
#include <map>
#include <string>
#include "AVTmathLib.h"
#include <sstream>

#define VERTICES 0
#define TEXCOORDS 2

// The storage for matrices
extern float mMatrix[COUNT_MATRICES][16];
extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];

// The normal matrix
extern float mNormal3x3[9];

class AlphaShader : public Shader {

private:
		
	GLint virtualColorID;
	GLint virtualDepthID;
	GLint realColorID;
	GLint realDepthID;
	GLint realSmoothDepthID;
	GLint coarseTrimapID;
	GLint trimapEdgeID;
	GLint realColorEdgeID;
	GLint unknownLabelsID;
	GLint finalTrimapID;

	GLint inputTextureSourceID;
	GLint inputTextureSource2ID;
	GLint iterationID;
	GLint modeID;
	GLint expandedForegroundID;
	GLint expandedBackgroundID;
	GLint expandedForegroundCostsID;
	GLint expandedBackgroundCostsID;


	GLint textureWidthID;
	GLint textureHeightID;

	GLint debugTextureID;
	GLint inputImageID;
	GLint outputImageID;
	GLint outputImage2ID;



public:
	AlphaShader(const std::string& vertexShader, const std::string& fragmentShader)
		: Shader(vertexShader, fragmentShader) {
		bindAttributes();
		getUniformLocations();
	}
	virtual ~AlphaShader() {
	}

	void bindAttributes(void) {
		bindAttribute(VERTICES, "position");
		bindAttribute(TEXCOORDS, "texCoord");
	}
	void getUniformLocations(void) {

		virtualColorID = getUniformLocation("virtualColor");
		virtualDepthID = getUniformLocation("virtualDepth");
		realColorID = getUniformLocation("realColor");
		realDepthID = getUniformLocation("realDepth");
		realSmoothDepthID = getUniformLocation("realSmoothDepth");
		coarseTrimapID = getUniformLocation("coarseTrimap");
		trimapEdgeID = getUniformLocation("trimapEdge");
		realColorEdgeID = getUniformLocation("realColorEdge");
		unknownLabelsID = getUniformLocation("unknownLabels");
		finalTrimapID = getUniformLocation("finalTrimap");
		inputTextureSourceID = getUniformLocation("inputTexture");
		inputTextureSource2ID = getUniformLocation("inputTexture2");
		iterationID = getUniformLocation("iteration");
		modeID = getUniformLocation("mode");
		expandedForegroundID = getUniformLocation("expandedForeground");
		expandedBackgroundID = getUniformLocation("expandedBackground");
		expandedForegroundCostsID = getUniformLocation("propagationCostsForeground");
		expandedBackgroundCostsID = getUniformLocation("propagationCostsBackground");

		textureWidthID = getUniformLocation("textureWidth");
		textureHeightID = getUniformLocation("textureHeight");

		inputImageID = getUniformLocation("inputImage");
		outputImageID = getUniformLocation("outputImage");
		outputImage2ID = getUniformLocation("outputImage2");

		debugTextureID = getUniformLocation("debugTexture");
	}
	void bindTextureUnits() {
		Shader::loadInt(virtualColorID, 0);
		Shader::loadInt(virtualDepthID, 1);
		Shader::loadInt(realColorID, 2);
		Shader::loadInt(realDepthID, 3);
		Shader::loadInt(realSmoothDepthID, 4);
		Shader::loadInt(coarseTrimapID, 5);
		Shader::loadInt(trimapEdgeID, 6);
		Shader::loadInt(realColorEdgeID, 7);
		Shader::loadInt(unknownLabelsID, 8);
		Shader::loadInt(finalTrimapID, 9);
		Shader::loadInt(expandedForegroundID, 14);
		Shader::loadInt(expandedBackgroundID, 15);
		Shader::loadInt(expandedForegroundCostsID, 16);
		Shader::loadInt(expandedBackgroundCostsID, 17);

		Shader::loadInt(debugTextureID, 10);
		
	}


	

	void loadVirtualColor(GLint id) {
		Shader::loadInt(virtualColorID, id);
	}
	void loadVirtualDepth(GLint id) {
		Shader::loadInt(virtualDepthID, id);
	}
	void loadRealColor(GLint id) {
		Shader::loadInt(realColorID, id);
	}
	void loadRealDepth(GLint id) {
		Shader::loadInt(realDepthID, id);
	}



	void loadInputTexture(int source) {
		Shader::loadInt(inputTextureSourceID, source);
	}
	void loadInputTexture2(int source) {
		Shader::loadInt(inputTextureSource2ID, source);
	}

	void loadInputImage(GLint i) {
		Shader::loadInt(inputImageID, i);
	}
	void loadOutputImage(GLint i) {
		Shader::loadInt(outputImageID, i);
	}
	void loadOutputImage2(GLint i) {
		Shader::loadInt(outputImage2ID, i);
	}


	void loadIteration(float i) {
		Shader::loadFloat(iterationID, i);
	}
	void loadMode(int mode) {
		Shader::loadInt(modeID, mode);
	}

	void loadTextureWidth(int width) {
		Shader::loadInt(textureWidthID, width);
	}
	void loadTextureHeight(int height) {
		Shader::loadInt(textureHeightID, height);
	}


	void loadMatrices() {
		computeDerivedMatrix(PROJ_VIEW_MODEL);
		loadViewModelMatrix(mCompMatrix[VIEW_MODEL]);
		loadProjViewModelMatrix(mCompMatrix[PROJ_VIEW_MODEL]);
	}


};



