#pragma once
#include "typeDefs.h"
#include "Vector.h"
#include "OpenGL.h"
extern uint _Width;
extern uint _Height;
extern float _screenRatio;
extern double mousePosX;
extern double mousePosY;
extern Vec3 bgColor;
extern uint charSizeSmall;
extern uint charSizeNormal;
extern uint charSizeLarge;
extern glm::mat4 transform;
extern glm::mat4 view;
extern glm::mat4 projection;