#pragma once
#include "TypeDefs.h"
#include "OpenGL.h"
#include "Objects.h"


void deleteObj(Object*& obj);
void deleteObj(uint index);
void deleteAll();
bool addGlobalObj(ObjectBase*& obj);
uint findObjSlot();
void setBgColor(Vec3 color);
void setBgColor(float r, float g, float b);
void drawAllObjs();
void addObjScript(Object*& obj, void* script, std::string scrName);
void removeObjScript(Object*& obj, unsigned int index);
unsigned int getObjScriptIndex(Object*& obj, std::string name);
void updateObjScripts(Object*& obj);
void clearObjScripts(Object*& obj);
void updateObjChildren(Object*& obj);
void* getObjScript(Object*& obj, std::string name);
void clearObjChildren(Object*& obj);
bool objCmp(const ObjectBase* obj1, const ObjectBase* obj2);
void drawObjStencil(Object* obj);

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

void start();
void update();
void end();

void windowMoveCallback(GLFWwindow* window, int xpos, int ypos);
void windowScaleCallback(GLFWwindow* window, int width, int height);
void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos);
void mouseClickCallback(GLFWwindow* window, int button, int action, int mods);
void keyPressCallback(GLFWwindow* window, int button, int scancode, int action, int mods);
void mouseScrollCallback(GLFWwindow* window, double xoff, double yoff);
#endif