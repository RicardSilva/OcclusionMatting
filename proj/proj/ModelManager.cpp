#include "ModelManager.h"

using namespace std;

ModelManager* ModelManager::_instance = nullptr;

void ModelManager::addModel(const string& name, ObjModel* model) {
	map.insert(make_pair(name, model));
}

ObjModel* ModelManager::getModel(const string& name) {
	return map[name];
}

ModelManager* ModelManager::instance() {
	if (_instance == nullptr)
		_instance = new ModelManager;
	return _instance;
}

void ModelManager::destroy() {
	for (auto element : map) {
		delete(element.second);
	}
	delete(_instance);
}
