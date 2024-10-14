#pragma once
#include "typeDefs.h"
#include "Vector.h"
#include "OpenGL.h"
extern glm::mat4 _transform;
extern glm::mat4 _view;
extern glm::mat4 _projection;
extern iVec2 _realMousePos;
extern iVec2 _windowPos;
extern Vec3 _bgColor;
extern uint _Width;
extern uint _Height;
extern double _mousePosX;
extern double _mousePosY;
extern double _deltaTime;
extern float _screenRatio;
extern bool _windowScaled;
extern bool _closeApp;
extern bool _lockMouse;
extern bool _hideMouse;
