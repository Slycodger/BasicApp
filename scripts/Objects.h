#pragma once
#if defined(_OBJECTS) || defined(_FLOW_CONTROL)
#include <map>
#include <stdexcept>
namespace Objects {
	void start();
	void end();
}
#endif

#include "TypeDefs.h"
#include "Vector.h"
#include <string>
#include <Set>
#include <Vector>
#include <limits>
#include "Shapes.h"
#include "MathConstants.h"

struct ObjectBase {
	uint index = 0;
	uint *texTarget = nullptr;
	uint *VBO = nullptr;
	uint *EBO = nullptr;
	uint *triCount = nullptr;
	std::vector<void*> scripts;
};

struct Transform {
	Vec3 position;
	Vec3 scale;
	Vec3 rotation;

	Transform() : position(0), scale(1), rotation(0) {}
};

struct Object : private ObjectBase { 
	Transform transform;
	Transform relativeTransform;
	Object* parent;
	std::set<Object*> children;
	Vec4 color;

	//--------------------------getter functions

	uint getIndex() const {
		return index;
	}
	uint getVBO() const {
		return *VBO;
	}
	uint getEBO() const {
		return *EBO;
	}
	uint getTriCount() const {
		return *triCount;
	}
	uint getTexture() {
		return *texTarget;
	}


	//-------------------------Setter functions

	void setTexture(uint& texture) {
		texTarget = &texture;
	}
	void removeTexture() {
		texTarget = nullptr;
	}
	

	//-------------------------Parenting functions

	void setParent(Object* obj) {
		if (parent == obj)
			return;
		if (parent != nullptr)
			parent->children.erase(this);
		obj->children.insert(this);
		parent = obj;
		relativeTransform.position = (transform.position - parent->transform.position) / parent->transform.scale;
		relativeTransform.scale = transform.scale / parent->transform.scale;
		relativeTransform.rotation = transform.rotation - parent->transform.rotation;
	}
	void setToRelative() {
		if (parent == nullptr)
			return;
		Vec3 angle = parent->transform.rotation + relativeTransform.rotation;
		transform.position.x = cos(angle.z * degToRad);
		transform.position.y = sin(angle.z * degToRad);
		transform.position *= (parent->transform.scale * relativeTransform.position).magnitude();
		transform.scale = relativeTransform.scale * parent->transform.scale;
		transform.position += parent->transform.position;
		transform.rotation = angle;
	}
	void removeParent() {
		parent->children.erase(this);
		parent = nullptr;
	}


	//------------------------------Checking functions

	bool usesTexture() {
		return texTarget == nullptr ? false : true;
	}


	//-------------------Constructor

	Object() : transform(), relativeTransform(), parent(nullptr), color(1) {}
};

Object* createObj(std::string objName);
bool unloadBufferObj(std::string name);
bool loadBufferObj(std::string name, uint* VBO, uint* EBO, uint* triCount);
void deleteObjMapping();
void createBufferObj(uint& VBO, uint& EBO, const float vertices[], const uint indices[], const size_t vertSize, const size_t indiceSize);