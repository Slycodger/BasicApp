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
	float yStart;
	float margin;
	uint mode;
	Vec4 fontColor;

	void start() override {
		thisObj->setTexture(texture);
		createTextTexture(texture, fontSize, lineSize, thisObj->transform.scale, yStart, margin, mode, font, text);
	}

	void update() override {
		thisObj->color = fontColor;

	}
	void end() override {
		glDeleteTextures(1, &texture);
	}

	void textUpdate() {
		createTextTexture(texture, fontSize, lineSize, thisObj->transform.scale, yStart, margin, mode, font, text);
	}

	TextBox() : text(), font(), fontSize(1), lineSize(1), yStart(1), margin(0), mode(0), fontColor(1), texture(0) {}

private :
	uint texture;
};

struct Button : scriptBase {
	Object* textObj;
	bool color = false;
	TextBox* textScr;

	void start() override {
		textObj = createObj("square");
		addObjScript(textObj, (void*)textScr);
		textObj->transform = thisObj->transform;
		textObj->depth = thisObj->depth + 0.05;
		textObj->setParent(thisObj);
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