#pragma once
#include "Objects.h"
#include "ObjScripts.h"
#include "Text.h"
#include "GlobalVars.h"
#include "FlowControl.h"
#include "Input.h"

struct TextBox : scriptBase {
	std::string text;
	std::string font;
	float fontSize;
	float lineSize;
	uint mode;
	Vec4 fontColor;

	void start() override {
		thisObj->setTexture(texture);
		tempText = text;
		tempFont = font;
		tempFontSize = fontSize;
		tempLineSize = lineSize;
		tempMode = mode;
		size = thisObj->transform.scale;
		createTextTexture(texture, fontSize, lineSize, size.x, size.y, mode, font, text);
	}
	void update() override {
		thisObj->color = fontColor;
		if (windowScaled || text != tempText || font != tempFont ||
		size != thisObj->transform.scale || fontSize != tempFontSize ||
		lineSize != tempLineSize || mode != tempMode) {
			tempText = text;
			tempFont = font;
			tempFontSize = fontSize;
			tempLineSize = lineSize;
			tempMode = mode;
			size = thisObj->transform.scale;
			createTextTexture(texture, fontSize, lineSize, size.x, size.y, mode, font, text);
		}
	}
	void end() override {
		glDeleteTextures(1, &texture);
	}

	TextBox() : text(), font(), fontSize(1), lineSize(1), mode(0), fontColor(1), texture(0), tempText(), tempFont(), size(1), tempFontSize(1), tempLineSize(1), tempMode(0) {}

private :
	uint texture;
	std::string tempText;
	std::string tempFont;
	Vec2 size;
	float tempFontSize;
	float tempLineSize;
	uint tempMode;
};

struct Button : scriptBase {
	Object* textObj;
	bool color = false;
	TextBox* textScr;

	void start() override {
		textObj = createObj("square");
		addObjScript(textObj, (void*)textScr);
		textObj->transform = thisObj->transform;
	}

	void update() override {
		float left = thisObj->transform.position.x - thisObj->transform.scale.x;
		float right = thisObj->transform.position.x + thisObj->transform.scale.x;
		float bottom = thisObj->transform.position.y - thisObj->transform.scale.y;
		float top = thisObj->transform.position.y + thisObj->transform.scale.y;
		if (keyAction::keyPressed(GLFW_MOUSE_BUTTON_1)) {
			if (mousePosX <= right && mousePosX >= left && mousePosY <= top && mousePosY >= bottom) {
				if (color) {
					thisObj->color = Vec4(1, 0, 0, 1);
					color = false;
				}
				else {
					thisObj->color = Vec4(0, 1, 0, 1);
					color = true;
				}
			}
		}


	}

	Button() : textObj(nullptr), textScr(new TextBox()) {}

};