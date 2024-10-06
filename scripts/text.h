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
#define TEXT_CENTER_RENDER 1


struct cChar {
	uint texTarget;
	Vec2 bearing;
	Vec2 size;
	float advance;

	cChar() : texTarget(0), bearing(0), size(0), advance(0) {}
	cChar(uint tex, Vec2 b, Vec2 s, float a) : texTarget(tex), bearing(b), size(s), advance(a) {}
};


bool loadFont(std::string font, std::string name);
cChar getChar(std::string name, char character);
void createTextTexture(uint& texture, float fontSize, float lineSize, int sharpness, float width, float height, uint mode, std::string font, std::string text);
void fontStart();
void fontEnd();