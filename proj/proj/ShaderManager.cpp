#include "ShaderManager.h"
using namespace std;

ShaderManager* ShaderManager::_instance = nullptr;

void ShaderManager::addShader(const string& name, Shader* shader) {
	map.insert(make_pair(name, shader));
}

Shader* ShaderManager::getShader(const string& name) {
	return map[name];
}

ShaderManager* ShaderManager::instance() {
	if (_instance == nullptr)
		_instance = new ShaderManager;
	return _instance;
}

void ShaderManager::destroy() {
	for (auto element : map) {
		delete(element.second);
	}
	delete(_instance);
}



