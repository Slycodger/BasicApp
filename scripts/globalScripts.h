#pragma once
#include "Objects.h"
#include "objScripts.h"
#include "text.h"
#include "globalVars.h"

struct TextBox : scriptBase {
	std::string text;
	std::string font;
	float fontSize;
	float lineSize;
	uint mode;
	Vec4 fontColor = 1;

	void start() override {
		thisObj->setTexture(texture);
		tempText = text;
		tempFont = font;
		tempFontSize = fontSize;
		tempLineSize = lineSize;
		tempMode = mode;
		size = thisObj->scale;
		createTextTexture(texture, fontSize, lineSize, 1, size.x, size.y, mode, font, text);
	}
	void update() override {
		thisObj->color = fontColor;
		if (windowScaled || text != tempText || font != tempFont ||
		size != thisObj->scale || fontSize != tempFontSize ||
		lineSize != tempLineSize || mode != tempMode) {
			tempText = text;
			tempFont = font;
			tempFontSize = fontSize;
			tempLineSize = lineSize;
			tempMode = mode;
			size = thisObj->scale;
			createTextTexture(texture, fontSize, lineSize, 2, size.x, size.y, mode, font, text);
		}
	}
	void end() override {
		glDeleteTextures(1, &texture);
	}

private :
	uint texture;
	std::string tempText;
	std::string tempFont;
	Vec2 size;
	float tempFontSize;
	float tempLineSize;
	uint tempMode;
};