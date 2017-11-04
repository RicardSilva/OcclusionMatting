#pragma once
#include "Shader.h"
#include <map>


class ShaderManager {

private:
	static ShaderManager* _instance;
	map<string, Shader*> map;
	ShaderManager() {};


public:

	void addShader(const string& name, Shader* shader);
	Shader* getShader(const string& name);

	static ShaderManager* instance();
	void destroy();

};
