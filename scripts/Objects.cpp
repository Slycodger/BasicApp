#define _OBJECTS
#include "Objects.h"

bool addGlobalObj(ObjectBase*&);

std::map<std::string, std::tuple<uint*, uint*, uint*>> objTypes;

//Creates the buffers for an object
void createBufferObj(uint& VBO, uint& EBO, const float vertices[], const uint indices[], const size_t vertSize, const size_t indiceSize) {
	glCreateBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertSize, vertices, GL_STATIC_DRAW);

	glCreateBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indiceSize, indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

//Loads an object into a map
bool loadBufferObj(std::string name, uint* VBO, uint* EBO, uint* triCount) {
	return objTypes.insert({ name, {VBO, EBO, triCount} }).second;
}

//Removes a specific object type from the map
bool unloadBufferObj(std::string name) {
	return objTypes.erase(name);
}

//Deletes all object types
void deleteObjMapping() {
	objTypes.clear();
}

//Returns a created object
Object* createObj(std::string objName) {
	try {
		auto objData = objTypes.at(objName);

		Object* obj = new Object;
		ObjectBase* objBase = (ObjectBase*)obj;

		objBase->VBO = std::get<0>(objData);
		objBase->EBO = std::get<1>(objData);
		objBase->triCount = std::get<2>(objData);

		if (!addGlobalObj(objBase))
		{
			delete(objBase);
			return nullptr;
		}

		obj->position = Vec2(0);
		obj->scale = Vec2(1);
		obj->rotation = 0;


		return obj;
	}
	catch (std::out_of_range) {
		return nullptr;
	}
}