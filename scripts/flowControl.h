#pragma once
#include "TypeDefs.h"
#include "OpenGL.h"
#include "Objects.h"


void deleteObj(Object*& obj);
void deleteObj(uint index);
void deleteAll();
void setBgColor(Vec4 color);
void setBgColor(float r, float g, float b, float a);
void addObjScript(Object*& obj, void* script);
void removeObjScript(Object*& obj, unsigned int index);
unsigned int getObjScriptIndex(Object*& obj, std::string name);
void updateObjScripts(Object*& obj);
void clearObjScripts(Object*& obj);
void updateObjChildren(Object*& obj);
void* getObjScript(Object*& obj, std::string name);
void clearObjChildren(Object*& obj);


#if defined(_FLOW_CONTROL) || defined(_PROGRAM_CONTROL)
#include "Shapes.h"
#include "IntializeShader.h"
#include "GlobalVars.h"
#include <iostream>
#include "Input.h"
#include "Constants.h"
#include "MathConstants.h"
#include "ObjScripts.h"
#include "Text.h"
#include <algorithm>
#include <windows.h>
#include "ProgUI.h"

void start();
void update();
void end();

void windowMoveCallback(GLFWwindow* window, int xpos, int ypos);
void windowScaleCallback(GLFWwindow* window, int width, int height);
void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos);
void mouseClickCallback(GLFWwindow* window, int button, int action, int mods);
void keyPressCallback(GLFWwindow* window, int button, int scancode, int action, int mods);
void mouseScrollCallback(GLFWwindow* window, double xoff, double yoff);
bool objCmp(const ObjectBase* obj1, const ObjectBase* obj2);
void drawObjStencil(Object* obj);
void drawAllObjs();
uint findObjSlot();
bool addGlobalObj(ObjectBase*& obj);
#endif