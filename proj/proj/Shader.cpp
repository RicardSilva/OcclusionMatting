#include "Shader.h"
#include <iostream>
#include <fstream>

using namespace std;

void Shader::loadFromString(GLenum type, const string& source) {
	GLuint shader = glCreateShader(type);

	const char * ptmp = source.c_str();
	glShaderSource(shader, 1, &ptmp, NULL);

	//check whether the shader loads fine
	GLint status;
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar *infoLog = new GLchar[infoLogLength];
		glGetShaderInfoLog(shader, infoLogLength, NULL, infoLog);
		GLchar *shaderType = "";
		if (type == GL_VERTEX_SHADER) shaderType = "Vertex Shader: ";
		if (type == GL_FRAGMENT_SHADER) shaderType = "Fragment Shader: ";
		cerr << shaderType << "Compile log: " << infoLog << endl;
		delete[] infoLog;
		glDeleteShader(shader);
	}
	shaders[totalShaders++] = shader;
}

GLuint Shader::createProgram() {
	program = glCreateProgram();
	if (shaders[VERTEX_SHADER] != 0) {
		glAttachShader(program, shaders[VERTEX_SHADER]);
	}
	if (shaders[FRAGMENT_SHADER] != 0) {
		glAttachShader(program, shaders[FRAGMENT_SHADER]);
	}

	return program;
}

void Shader::linkProgram() {
	//link and check whether the program links fine
	GLint status;
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		GLint infoLogLength;

		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar *infoLog = new GLchar[infoLogLength];
		glGetProgramInfoLog(program, infoLogLength, NULL, infoLog);
		cerr << "Link log: " << infoLog << endl;
		delete[] infoLog;

		glDeleteProgram(program);
		glDeleteShader(shaders[VERTEX_SHADER]);
		glDeleteShader(shaders[FRAGMENT_SHADER]);

	}

}

void Shader::use() {
	glUseProgram(program);
}

void Shader::unUse() {
	glUseProgram(0);
}

void Shader::loadInt(GLint location, GLint i) {
	glUniform1i(location, i);
}
void Shader::loadFloat(GLint location, GLfloat f) {
	glUniform1f(location, f);
}
void Shader::loadBool(GLint location, bool b) {
	glUniform1f(location, b);
}
void Shader::loadVec3(GLint location, vec3 &v) {
	glUniform3fv(location, 1, v.toPointer());
}
void Shader::loadVec4(GLint location, vec4 &v) {
	glUniform4fv(location, 1, v.toPointer());
}
void Shader::loadMat3(GLint location, float* m) {
	glUniformMatrix3fv(location, 1, GL_FALSE, m);
}
void Shader::loadMat4(GLint location, float* m) {
	glUniformMatrix4fv(location, 1, GL_FALSE, m);
}
//void Shader::LoadTexture(GLint location, Texture *t) {
//	glUniform1i(location, 0);
//}


void Shader::bindAttribute(GLint attribute, const GLchar* variableName) {
	glBindAttribLocation(program, attribute, variableName);
}
GLint Shader::getUniformLocation(const GLchar* source) {
	return glGetUniformLocation(program, source);
}


GLint Shader::getUniformBlockIndex(const GLchar* source) {
	return glGetUniformBlockIndex(program, source);
}


void Shader::deleteProgram() {
	unUse();
	glDetachShader(program, shaders[VERTEX_SHADER]);
	glDetachShader(program, shaders[FRAGMENT_SHADER]);

	glDeleteShader(shaders[VERTEX_SHADER]);
	glDeleteShader(shaders[FRAGMENT_SHADER]);

	glDeleteProgram(program); program = -1;
}


void Shader::loadFromFile(GLenum whichShader, const string& filename) {
	ifstream fp;
	fp.open(filename.c_str(), ios_base::in);
	if (fp) {

		string buffer(std::istreambuf_iterator<char>(fp), (std::istreambuf_iterator<char>()));
		//copy to source
		loadFromString(whichShader, buffer);
	}
	else {
		cerr << "Error loading shader: " << filename << endl;
	}
}

