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
#include <iostream>
#include "MathConstants.h"

struct ObjectBase {
	uint index = 0;
	uint *texTarget = nullptr;
	uint *VBO = nullptr;
	uint *EBO = nullptr;
	uint *triCount = nullptr;
	std::vector<void*> scripts;
	float depth = 0;
	float relativeDepth = 0;
	ObjectBase() : index(0), texTarget(nullptr), VBO(nullptr), EBO(nullptr), triCount(nullptr), scripts(), depth(0), relativeDepth(0) {}
};

struct Transform {
	Vec2 position = 0;
	Vec2 scale = 1;
	float rotation = 0;

	Transform() : position(0), scale(1), rotation(0) {}
};

struct Object : private ObjectBase { 
	Transform transform;
	Transform relativeTransform;
	std::set<Object*> children;
    std::set<Object*> dependencies;
    Object* dependent = nullptr;
	Object* parent = nullptr;
	Vec4 color = 1;
	bool weak = false;
	bool active = true;


	//-------------------Constructor
    //-------------------Destructor

	Object() : transform(), relativeTransform(), children(), dependencies(), parent(nullptr), color(1), weak(false), active(true) {}
    ~Object() {}

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
	float getDepth() {
		return depth;
	}
	float getRelativeDepth() {
		return relativeDepth;
	}


	//-------------------------Setter functions

	void setTexture(uint& texture) {
		texTarget = &texture;
	}
	void removeTexture() {
		texTarget = nullptr;
	}
	void setDepth(float nDepth) {
		depth = nDepth;
	}
	void setRelativeDepth(float nDepth) {
		relativeDepth = nDepth;
	}
	void addDepth(float nDepth) {
		depth += nDepth;
	}
	void addRelativeDepth(float nDepth) {
		relativeDepth += nDepth;
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
		relativeDepth = depth - parent->depth;
	}
	void setToRelative() {
		if (parent == nullptr)
			return;
		float angle = parent->transform.rotation + relativeTransform.rotation;
		transform.scale = relativeTransform.scale * parent->transform.scale;
		transform.position = relativeTransform.position * parent->transform.scale;

		transform.position.x = transform.position.x * cos(angle * degToRad) - transform.position.y * sin(angle * degToRad);
		transform.position.y = transform.position.y * cos(angle * degToRad) + transform.position.x * sin(angle * degToRad);

		transform.position += parent->transform.position;


		transform.rotation = angle;
		depth = relativeDepth + parent->depth;
	}
	void removeParent() {
		if (parent == nullptr)
			return;
		parent->children.erase(this);
		parent = nullptr;
	}


    //Dependent object stuff

    void setDependent(Object* obj) {
        removeDependent();

        dependent = obj;
        obj->dependencies.insert(this);
    }

    void removeDependent() {
        if (dependent == nullptr)
            return;
        dependent->dependencies.erase(this);
        dependent = nullptr;
    }



	//------------------------------Checking functions

	bool usesTexture() {
		return texTarget == nullptr ? false : true;
	}
	static bool parentOff(Object*& obj) {
		if (obj->parent == nullptr)
			return false;
		if (!obj->parent->active)
			return true;
		return parentOff(obj->parent);
	}
};

Object* createObj(std::string objName);
bool unloadBufferObj(std::string name);
bool loadBufferObj(std::string name, uint* VBO, uint* EBO, uint* triCount);
void deleteObjMapping();
void createBufferObj(uint& VBO, uint& EBO, const float vertices[], const uint indices[], const size_t vertSize, const size_t indiceSize);