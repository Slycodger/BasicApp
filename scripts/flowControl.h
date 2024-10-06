#pragma once
#include "typeDefs.h"
#include "OpenGL.h"
#include "Objects.h"

#ifdef _FLOW_CONTROL
#include "Shapes.h"
#include "IntializeShader.h"
#include "globalVars.h"
#include <iostream>
#include "input.h"
#include "Constants.h"
#include "MathConstants.h"
#include "objScripts.h"
#include "text.h"
#endif

void start();
void update();
void windowScaleCallback(GLFWwindow* window, int width, int height);
void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos);
void mouseClickCallback(GLFWwindow* window, int button, int action, int mods);
void keyPressCallback(GLFWwindow* window, int button, int scancode, int action, int mods);
void mouseScrollCallback(GLFWwindow* window, double xoff, double yoff);
void deleteObj(Object*& obj);
void deleteObj(uint index);
void deleteAll();
bool addGlobalObj(ObjectBase*& obj);
uint findObjSlot();
void end();
void setBgColor(Vec3 color);
void setBgColor(float r, float g, float b);
void drawAllObjs();
void addObjScript(Object*& obj, void* script);
void removeObjScript(Object*& obj, unsigned int index);
unsigned int getObjScriptIndex(Object*& obj, std::string name);
void updateObjScripts(Object*& obj);
void clearObjScripts(Object*& obj);