#pragma once
#if defined(_OBJECTS) || defined(_FLOW_CONTROL)
#include <map>
#include <stdexcept>
#include "Constants.h"
#include <algorithm>
#include "IntializeShader.h"
#include "OpenGL.h"
#include "GlobalVars.h"
namespace Objects {
	void start();
	void end();
}
#endif

#include "ObjScripts.h"
#include "TypeDefs.h"
#include "Vector.h"
#include <Set>
#include <string>
#include <Vector>
#include <limits>
#include "Shapes.h"
#include <iostream>
#include "MathConstants.h"
#include <type_traits>

struct ObjectBase {
	int index = -1;
	uint *texTarget = nullptr;
	uint *VBO = nullptr;
	uint *EBO = nullptr;
	uint *triCount = nullptr;
	std::vector<void*> scripts;
	float depth = 0;
	float relativeDepth = 0;
	ObjectBase() : index(-1), texTarget(nullptr), VBO(nullptr), EBO(nullptr), triCount(nullptr), scripts(), depth(0), relativeDepth(0) {}
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
    std::string objType;
    bool scriptCreated = false;
    

    Object& operator=(Object* obj) {
        this->transform = obj->transform;

        void addObjScript(Object * obj, void* script, char c);
        for (auto& scrV : ((ObjectBase*)obj)->scripts) {
            void* newScr = ((scriptBase*)scrV)->getNew();
            addObjScript(this, newScr, 'c');
        }

        for (auto child : obj->children) {
            if (child->scriptCreated)
                continue;
            Object* temp = new Object;
            *temp = child;
            temp->setParent(this);
        }

        this->color = obj->color;
        this->weak = obj->weak;
        this->active = obj->active;
        this->objType = obj->objType;

        ObjectBase* base = (ObjectBase*)this;
        ObjectBase* objBase = (ObjectBase*)obj;

        base->texTarget = objBase->texTarget;
        base->VBO = objBase->VBO;
        base->EBO = objBase->EBO;
        base->triCount = objBase->triCount;
        base->depth = objBase->depth;
        base->relativeDepth = objBase->relativeDepth;

        return *this;
    }


	//-------------------Constructor
    //-------------------Destructor

	Object() : transform(), relativeTransform(), children(), dependencies(), parent(nullptr), color(1), weak(false), active(true), scriptCreated(false) {}
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
    
    template <typename T>
    T* getScript() {
        for (auto scrV : ((ObjectBase*)this)->scripts) {
            scriptBase* scr = (scriptBase*)scrV;
            if (scr->getName() == T::name)
                return (T*)scrV;
        }
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

		transform.position.x = relativeTransform.position.x * parent->transform.scale.x * cos(angle * degToRad) - relativeTransform.position.y * parent->transform.scale.y * sin(angle * degToRad);
		transform.position.y = relativeTransform.position.y * parent->transform.scale.y * cos(angle * degToRad) + relativeTransform.position.x * parent->transform.scale.x * sin(angle * degToRad);

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
	static bool chainActive(Object*& obj) {
        if (!obj->active)
            return false;
		if (obj->parent == nullptr)
			return true;
		if (!obj->parent->active)
			return false;
		return chainActive(obj->parent);
	}
};

Object* createObj(std::string objName);
bool unloadBufferObj(std::string name);
bool loadBufferObj(std::string name, uint* VBO, uint* EBO, uint* triCount);
void deleteObjMapping();
void createBufferObj(uint& VBO, uint& EBO, const float vertices[], const uint indices[], const size_t vertSize, const size_t indiceSize);
void addObjScript(Object* obj, void* script);
void removeObjScript(Object* obj, unsigned int index);
unsigned int getObjScriptIndex(Object*& obj, std::string name);
void updateObjScripts(Object*& obj);
void clearObjScripts(Object* const& obj);
void updateObjChildren(Object*& obj);
void clearObjChildren(Object*& obj);
void deleteObj(Object*& obj);
void deleteObj(Object*& obj, int);
void deleteAll();
bool objCmp(const ObjectBase* obj1, const ObjectBase* obj2);
void drawObjStencil(Object* obj);
void drawAllObjs();
uint findObjSlot();
bool addGlobalObj(ObjectBase*& obj);
void changeProjectionToOrtho(float width, float near, float depth);
void changeProjectionToPerp(float FOV, float near, float depth);
Object* createObj();
Object* instantiateObj(std::string objName);
bool saveObj(Object* obj, std::string objName);
void addObjScript(Object* obj, void* script, char c);
void deleteObj(Object*& obj, const char);
void startObject(Object* obj);
void createScriptObjs(std::pair<std::vector<std::string>, std::vector<void*>&> pair);

extern ObjectBase* globalObjects[];
extern uint objCount;