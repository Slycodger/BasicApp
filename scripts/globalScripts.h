#pragma once
#include "Objects.h"
#include "ObjScripts.h"
#include "Text.h"
#include "GlobalVars.h"
#include "FlowControl.h"
#include "Input.h"
#include <typeinfo>

struct TextBox : protected scriptBase {
	std::string text;
	std::string font;
	float fontSize = 1;
	float lineSize = 1;
	float yStart = 1;
	float margin = 0;
	uint mode = 0;
	Vec4 fontColor = 1;

	void start() override {
		thisObj->setTexture(texture);
		createTextTexture(texture, fontSize, lineSize, thisObj->transform.scale, yStart, margin, mode, font, text);
	}

	void update() override {
		thisObj->color = fontColor;

	}
	void end() override {
		glDeleteTextures(1, &texture);
		text.clear();
		font.clear();
	}

	void textUpdate() {
		createTextTexture(texture, fontSize, lineSize, thisObj->transform.scale, yStart, margin, mode, font, text);
	}

	TextBox() {}

private :
	uint texture = 0;
};


struct Button : protected scriptBase {
	Object* textObj = nullptr;
	TextBox* textScr = nullptr;
	bool held = false;

	void start() override {
		textScr = new TextBox();

		textObj = createObj("square");
		textScr->text = "Button";
		textScr->font = "CascadiaCode_SMALL";
		textScr->fontSize = 0.5;
		textScr->lineSize = 0.55;
		textScr->mode = TEXT_CENTER_RENDER;
		textScr->fontColor = Vec4(0, 0, 0, 1);


		std::string textName = typeid(textScr).name();
		textObj->transform = thisObj->transform;
		textObj->addDepth(0.05f);
		textObj->setParent(thisObj);

		addObjScript(textObj, (void*)textScr, textName.substr(textName.find(' ') + 1));

		cStart();
	}


	void update() override {
		if (keyAction::keyReleased(GLFW_MOUSE_BUTTON_1) && held) {
			held = false;
				onReleased();
			return;
		}

		float left = thisObj->transform.position.x - thisObj->transform.scale.x;
		float right = thisObj->transform.position.x + thisObj->transform.scale.x;
		float bottom = thisObj->transform.position.y - thisObj->transform.scale.y;
		float top = thisObj->transform.position.y + thisObj->transform.scale.y;
		if (keyAction::keyPressed(GLFW_MOUSE_BUTTON_1)) {
			if (_mousePosX <= right && _mousePosX >= left && _mousePosY <= top && _mousePosY >= bottom) {
					onPressed();
				held = true;
			}
		}

		if (held)
			onHeld();

		cUpdate();
	}

	virtual void cStart() {}
	virtual void cUpdate() {}
	virtual void onPressed() {}
	virtual void onHeld() {}
	virtual void onReleased() {}

	Button() {}
};

struct TextField : public Button {
	bool insideOf = false;
	std::string text = "";
	std::string emptyText = "Enter text here...";
	Vec4 emptyColor = Vec4(0.7, 0.7, 0.7, 1);
	Vec4 color = Vec4(0, 0, 0, 1);

	void cStart() override {
		textScr->text = emptyText;
		textScr->fontColor = emptyColor;
		textScr->mode = TEXT_LINE_LEFT_RENDER;
		textScr->textUpdate();
	}

	void cUpdate() override {
		if (keyAction::keyPressed(GLFW_KEY_ESCAPE)) {
			insideOf = false;
			_hideMouse = false;
			_lockMouse = false;
		}

		if (insideOf) {
			textScr->fontColor = color;
			keyAction::buildText(text);
			if (text != textScr->text) {
				textScr->text = text;
				textScr->textUpdate();
			}
		}
		else if (text == "") {
			textScr->text = emptyText;
			textScr->fontColor = emptyColor;
			textScr->textUpdate();
		}
	}

	void onPressed() override {
		_hideMouse = true;
		_lockMouse = true;
		insideOf = true;
	}

	void end() override {
		text.clear();
	}

	TextField() {}
};