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
	GLint foregroundColorID;



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
		foregroundColorID = getUniformLocation("foregroundColor");
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
		Shader::loadInt(foregroundColorID, 10);
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

	void loadMatrices() {
		computeDerivedMatrix(PROJ_VIEW_MODEL);
		loadViewModelMatrix(mCompMatrix[VIEW_MODEL]);
		loadProjViewModelMatrix(mCompMatrix[PROJ_VIEW_MODEL]);
	}


};



