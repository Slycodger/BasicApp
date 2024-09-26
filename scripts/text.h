#pragma once
#ifdef _TEXT
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftoutln.h>
#include "OpenGL.h"
#include <map>
#include "globalVars.h"
#include "constants.h"
#include "IntializeShader.h"
#include "Shapes.h"
#endif

#include "typeDefs.h"
#include "Vector.h"
#include <string>

#define TEXT_LEFT_RENDER 0
#define TEXT_RIGHT_RENDER 1
#define TEXT_CENTER_RENDER 2
#define TEXT_LEFT_STRAIGHT_RENDER 3;
#define TEXT_RIGHT_STRAIGHT_RENDER 4;

struct cChar {
	uint texTarget;
	Vec2 bearing;
	Vec2 size;
	int advance;

	cChar() : texTarget(0), bearing(0), size(0), advance(0) {}
	cChar(uint tex, Vec2 b, Vec2 s, float a) : texTarget(tex), bearing(b), size(s), advance(a) {}
};


bool loadFont(std::string font, std::string name);
cChar getChar(std::string name, char character);
void createTextTexture(uint& texture, std::string text, float lineSize, float width, float height, uint mode);
void fontStart();
void fontEnd();