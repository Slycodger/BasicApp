#pragma once
#include "Objects.h"
#include "ObjScripts.h"
#include "Text.h"
#include "GlobalVars.h"
#include "Input.h"
#ifndef _FLOW_CONTROL
#include "FlowControl.h"
#endif

struct TextBox : public scriptBase {
    constexpr static const char* name = "TextBox";
    Object* thisObj = nullptr;

	std::string text;
	std::string font = "CascadiaCode_NORMAL";
	float fontSize = 1;
	float lineSize = 1;
	float yStart = 1;
	float margin = 0;
	uint mode = 0;
	Vec4 fontColor = 1;

	void start() override {
        thisObj = (Object*)vThisObj;
        scrName = name;

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
		text.~basic_string();
		font.~basic_string();
	}

	void textUpdate() {
		createTextTexture(texture, fontSize, lineSize, thisObj->transform.scale, yStart, margin, mode, font, text);
	}

    TextBox* operator =(const TextBox& val) {
        this->text = val.text;
        this->font = val.font;
        this->fontSize = val.fontSize;
        this->lineSize = val.lineSize;
        this->yStart = val.yStart;
        this->margin = val.margin;
        this->mode = val.mode;
        this->fontColor = val.fontColor;
        return this;
    }

	TextBox() {}

private :
	uint texture = 0;
};


struct ButtonMain : public scriptBase {
    constexpr static const char* name = "ButtonMain";
    Object* thisObj = nullptr;

	Object* textObj = nullptr;
	TextBox* textScr = nullptr;
	bool held = false;

	void start() override {
        thisObj = (Object*)vThisObj;
        scrName = name;

		textScr = new TextBox();

		textObj = createObj("square");
		textScr->text = "Button";
		textScr->font = "CascadiaCode_SMALL";
		textScr->fontSize = 0.5;
		textScr->lineSize = 0.55;
		textScr->mode = TEXT_CENTER_RENDER;
		textScr->fontColor = Vec4(0, 0, 0, 1);


		textObj->transform = thisObj->transform;
        textObj->setParent(thisObj);
        textObj->setRelativeDepth(0.05f);
        textObj->setToRelative();

        addObjScript(textObj, textScr);

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

	ButtonMain() {}
};


struct Button : public scriptBase {
    constexpr static const char* name = "Button";
    Object* thisObj = nullptr;

	Object* textObj = nullptr;
	TextBox* textScr = nullptr;
	bool held = false;

	void (*onPressed)() = nullptr;
	void (*BonPressed)(Button*) = nullptr;
	void (*onHeld)() = nullptr;
	void (*BonHeld)(Button*) = nullptr;
	void (*onReleased)() = nullptr;
	void (*BonReleased)(Button*) = nullptr;

	void start() override {
        thisObj = (Object*)vThisObj;

        scrName = name;

		textScr = new TextBox();

		textObj = createObj("square");
		textScr->text = "Button";
		textScr->font = "CascadiaCode_NORMAL";
		textScr->fontSize = 0.5;
		textScr->lineSize = 0.55;
		textScr->mode = TEXT_CENTER_RENDER;
		textScr->fontColor = Vec4(0, 0, 0, 1);


		textObj->transform = thisObj->transform;
        textObj->setParent(thisObj);
        textObj->setRelativeDepth(0.05f);

		addObjScript(textObj, textScr);
	}


	void update() override {
		if (keyAction::keyReleased(GLFW_MOUSE_BUTTON_1) && held) {
			held = false;
            if (onReleased)
                onReleased();
            else if (BonReleased)
                BonReleased(this);
			return;
		}

		float left = thisObj->transform.position.x - thisObj->transform.scale.x;
		float right = thisObj->transform.position.x + thisObj->transform.scale.x;
		float bottom = thisObj->transform.position.y - thisObj->transform.scale.y;
		float top = thisObj->transform.position.y + thisObj->transform.scale.y;
		if (keyAction::keyPressed(GLFW_MOUSE_BUTTON_1)) {
			if (_mousePosX <= right && _mousePosX >= left && _mousePosY <= top && _mousePosY >= bottom) {
                if (onPressed)
                    onPressed();
               else if (BonPressed)
                    BonPressed(this);
				held = true;
			}
		}
		if (held)
            if (onHeld)
                onHeld();
            else if (BonHeld)
                BonHeld(this);
	}

	Button() {}
};


struct VoidButton : public scriptBase {
    constexpr static const char* name = "VoidButton";
    Object* thisObj = nullptr;

    bool inParent = false;

    Object* textObj = nullptr;
    TextBox* textScr = nullptr;
    bool held = false;

	void* pressVal = nullptr;
	void* heldVal = nullptr;
	void* releaseVal = nullptr;

	void (*pressDelete)(void* val) = nullptr;
	void (*heldDelete)(void* val) = nullptr;
	void (*releaseDelete)(void* val) = nullptr;

	void (*onPressed)(void* val) = nullptr;
	void (*onHeld)(void* val) = nullptr;
	void (*onReleased)(void* val) = nullptr;

	void start() override {
        thisObj = (Object*)vThisObj;

        scrName = name;

        textScr = new TextBox();

		textObj = createObj("square");
		textScr->text = "Button";
		textScr->font = "CascadiaCode_SMALL";
		textScr->fontSize = 0.5;
		textScr->lineSize = 0.55;
		textScr->mode = TEXT_CENTER_RENDER;
		textScr->fontColor = Vec4(0, 0, 0, 1);


		textObj->transform = thisObj->transform;
		textObj->setParent(thisObj);
        textObj->setRelativeDepth(0.05f);
        textObj->setToRelative();

        addObjScript(textObj, textScr);
	}


	void update() override {
		if (keyAction::keyReleased(GLFW_MOUSE_BUTTON_1) && held) {
			held = false;
			if (onReleased)
				onReleased(releaseVal);
			return;
		}

		float left = thisObj->transform.position.x - thisObj->transform.scale.x;
		float right = thisObj->transform.position.x + thisObj->transform.scale.x;
		float bottom = thisObj->transform.position.y - thisObj->transform.scale.y;
		float top = thisObj->transform.position.y + thisObj->transform.scale.y;

        if (inParent && thisObj->parent != nullptr) {
            float pLeft = thisObj->parent->transform.position.x - thisObj->parent->transform.scale.x;
            float pRight = thisObj->parent->transform.position.x + thisObj->parent->transform.scale.x;
            float pBottom = thisObj->parent->transform.position.y - thisObj->parent->transform.scale.y;
            float pTop = thisObj->parent->transform.position.y + thisObj->parent->transform.scale.y;

            if (keyAction::keyPressed(GLFW_MOUSE_BUTTON_1)) {
                if (_mousePosX <= right && _mousePosX >= left && _mousePosY <= top && _mousePosY >= bottom && _mousePosX <= pRight && _mousePosX >= pLeft && _mousePosY <= pTop && _mousePosY >= pBottom) {
                    if (onPressed)
                        onPressed(pressVal);
                    held = true;
                }
            }
        } else if (keyAction::keyPressed(GLFW_MOUSE_BUTTON_1)) {
			if (_mousePosX <= right && _mousePosX >= left && _mousePosY <= top && _mousePosY >= bottom) {
				if (onPressed)
					onPressed(pressVal);
				held = true;
			}
		}

		if (onHeld && held)
			onHeld(heldVal);
	}

	void end() override {
		if (pressDelete)
			pressDelete(pressVal);
		if (heldDelete)
			heldDelete(heldVal);
		if (releaseDelete)
			releaseDelete(releaseVal);
	}


	VoidButton() {}
};


struct TextField : public ButtonMain {
    constexpr static const char* name = "TextField";

	bool insideOf = false;
	std::string text = "";
	std::string emptyText = "Enter text here...";
	Vec4 emptyColor = Vec4(0.7, 0.7, 0.7, 1);
	Vec4 color = Vec4(0, 0, 0, 1);

	void cStart() override {
        scrName = name;

		textScr->text = emptyText;
		textScr->fontColor = emptyColor;
		textScr->mode = TEXT_LINE_LEFT_RENDER;
		textScr->textUpdate();
	}

	void cUpdate() override {
		giveKeyAction::setIgnore(false);
		if (keyAction::keyPressed(GLFW_KEY_ESCAPE)) {
			insideOf = false;
			_hideMouse = false;
			_lockMouse = false;
		}

		if (insideOf) {
			textScr->fontColor = color;
			keyAction::buildText(text);
			giveKeyAction::setIgnore(true);
			if (text != textScr->text) {
				textScr->text = text;
				textScr->textUpdate();
			}
		}
		else if (textScr->text != emptyText && text == "") {
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
		text.~basic_string();
		emptyText.clear();
		emptyText.~basic_string();
	}

	TextField() {}
};


struct DropDownFieldDynamic : public ButtonMain { 
    constexpr static const char* name = "DropDownFieldDynamic";

    std::string unknownMessage = "Select Option";
	float optionHeight = 0.125f;
	float optionOffset = 0.01f;
	Vec2 newObjPos = 0;
	std::string choice;
	TextBox buttonText;
    Object* background = nullptr;
    std::vector<Object*> optionObjs;

	void onPressed() override {
        background->active = !background->active;
        for (auto& obj : optionObjs)
            obj->active = !obj->active;
	}

	void cStart() override {
        scrName = name;

        textScr->text = unknownMessage;
        textScr->textUpdate();

        background = createObj("square");
        background->transform.position = thisObj->transform.position;
        background->transform.position.y -= thisObj->transform.scale.y;
        background->transform.scale = { thisObj->transform.scale.x, 0 };
        background->setDepth(thisObj->getDepth() + 0.05f);
        background->color = Vec4{ 0.7, 0.7, 0.7, 1 };

        newObjPos = background->transform.position;
        newObjPos.y += optionHeight;

        addOption(unknownMessage);

        onPressed();
	}

	void addOption(std::string option) {
        background->transform.scale.y += optionHeight + optionOffset / 2;
        background->transform.position.y -= optionHeight + optionOffset / 2;
        background->setToRelative();
        newObjPos.y -= optionHeight * 2 + optionOffset;

        Object* newOption = createObj("square");
        VoidButton* newScr = new VoidButton;

        void dynamicDropDownFunction(void*);
        newScr->onPressed = dynamicDropDownFunction;
        newScr->pressVal = (void*)new std::pair<DropDownFieldDynamic*, std::string>(this, option);
        void dynamicDropDownDelete(void* val);
        newScr->pressDelete = dynamicDropDownDelete;

        newOption->transform.position = newObjPos;
        newOption->transform.scale = { thisObj->transform.scale.x, optionHeight };
        addObjScript(newOption, newScr);
        newOption->setDepth(background->getDepth() + 0.05f);
        *newScr->textScr = buttonText;
        newScr->textScr->text = option;
        newScr->textScr->textUpdate();


        optionObjs.push_back(newOption);

	}

	void selectOption(std::string option) {
		textScr->text = option;
		textScr->textUpdate();
        onPressed();
        choice = option;
	}

	void end() override {
		optionObjs.clear();
		choice.~basic_string();
	}
};


struct DropDownFieldStatic : public ButtonMain {
    constexpr static const char* name = "DropDownFieldStatic";

    float height = 0.5f;
    float optionHeight = 0.125f;
    float optionOffset = 0.01f;
    float objOffset = 0;
    float scrollSpeed = 0.1f;
    float lowest = 0;
    std::string unknownMessage = "Select Option";
    std::string choice;
    TextBox buttonText;
    Object* background = nullptr;
    std::vector<std::pair<Object*, float>> optionObjs;

    void onPressed() override {
        background->active = !background->active;
        objOffset = 0;
    }

    void cStart() override {
        scrName = name;

        textScr->text = unknownMessage;
        textScr->textUpdate();

        background = createObj("square");
        background->transform.position = thisObj->transform.position;
        background->transform.position.y -= thisObj->transform.scale.y + height;
        background->transform.scale = { thisObj->transform.scale.x, height };
        background->setParent(thisObj);
        background->setToRelative();
        background->color = Vec4{ 0.7, 0.7, 0.7, 1 };

        addOption(unknownMessage);

        onPressed();

    }

    void cUpdate() override {
        if (keyAction::scrollUp() && objOffset > 0)
            objOffset -= scrollSpeed;
        if (keyAction::scrollDown() && objOffset < optionObjs.size() * (optionHeight + optionOffset))
            objOffset += scrollSpeed;

        for (int i = 0; i < optionObjs.size(); i++)
            std::get<0>(optionObjs[i])->relativeTransform.position.y = 
            (i * (optionHeight + optionOffset)) + 
            thisObj->transform.position.y - thisObj->transform.scale.y +
            objOffset;
    }

    void addOption(std::string option) {
        Object* newOption = createObj("square");
        VoidButton* newScr = new VoidButton;
        newScr->inParent = true;

        void staticDropDownFunction(void*);
        newScr->onPressed = staticDropDownFunction;
        newScr->pressVal = (void*)new std::pair<DropDownFieldStatic*, std::string>(this, option);
        void staticDropDownDelete(void* val);
        newScr->pressDelete = staticDropDownDelete;

        newOption->transform.position = { thisObj->transform.position.x, thisObj->transform.position.y };
        newOption->transform.scale = { thisObj->transform.scale.x, optionHeight };
        newOption->setParent(background);
        newOption->setDependent(background);
        newOption->setRelativeDepth(0.05f);
        newOption->setToRelative();
        addObjScript(newOption, newScr);
        newScr->textObj->setDependent(background);
        *newScr->textScr = buttonText;
        newScr->textScr->text = option;
        newScr->textScr->textUpdate();

        optionObjs.push_back(std::pair<Object*, float> (newOption, newOption->relativeTransform.position.y));
    }

    void removeOption(int index) {
        deleteObj(std::get<0>(optionObjs[index]));
        optionObjs.erase(optionObjs.begin() + index);
    }

    void selectOption(std::string option) {
        textScr->text = option;
        textScr->textUpdate();
        onPressed();
        choice = option;
    }
};

