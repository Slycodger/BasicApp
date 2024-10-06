#pragma once
#ifdef _OBJECTS
#include <map>
#include <stdexcept>
#endif

#include "typeDefs.h"
#include "Vector.h"
#include <string>
#include <Vector>
#include <limits>

struct ObjectBase {
	uint index = 0;
	uint *texTarget = nullptr;
	uint *VBO = nullptr;
	uint *EBO = nullptr;
	uint *triCount = nullptr;
	std::vector<void*> scripts;
};

struct Object : private ObjectBase { 
	Vec2 position;
	Vec2 scale;
	float rotation;
	Vec4 color;


	uint getIndex() {
		return index;
	}
	uint getVBO() {
		return *VBO;
	}
	uint getEBO() {
		return *EBO;
	}
	uint getTriCount() {
		return *triCount;
	}
	void setTexture(uint& texture) {
		texTarget = &texture;
	}
	void removeTexture() {
		texTarget = nullptr;
	}
	bool usesTexture() {
		return texTarget == nullptr ? false : true;
	}
	uint getTexture() {
		return *texTarget;
	}



	Object() : position(0), scale(1), rotation(0), color(1) {}
};

Object* createObj(std::string objName);
bool unloadBufferObj(std::string name);
bool loadBufferObj(std::string name, uint* VBO, uint* EBO, uint* triCount);
void deleteObjMapping();
void createBufferObj(uint& VBO, uint& EBO, const float vertices[], const uint indices[], const size_t vertSize, const size_t indiceSize);