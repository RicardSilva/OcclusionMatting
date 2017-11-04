#pragma once
#include "Mesh.h"
#include <map>

using namespace std;

class ModelManager {

private:
	static ModelManager* _instance;
	map<string, ObjModel*> map;
	ModelManager() {};


public:

	void addModel(const string& name, ObjModel* model);
	ObjModel* getModel(const string& name);

	static ModelManager* instance();
	void destroy();

};
