static const char* const sponge = "Hey, all you Goobers\nIt's time to say howdy to your favorite undersea peanut, Goofy Goober\n(Yeah)\n(Yeah)\n(Yeah)\n\nAlright, folks, this one goes out to my two bestest friends in the whole world\nPatrick and this big peanut guy\nIt's a little ditty called\nGoofy Goober (yeah)\n\nOh, I'm a goofy goober, yeah\nYou're a goofy goober, yeah\nWe're all goofy goobers, yeah\nGoofy, goofy, goober, goober, yeah (yeah)\nI'm a goofy goober, yeah\nYou're a goofy goober, yeah\nWe're all goofy goobers, yeah\nGoofy, goofy, goober, goober, yeah (yeah)\n\nDJ (yeah), time for the test (yeah)\nNo baby can resist singin' along to this (yeah)\n(Yeah, yeah)\n(Yeah, yeah)\n(Yeah, yeah)\n\nSpongeBob, it's the Goofy Goober theme song\nI know\n\nOh, I'm a goofy goober, yeah\nYou're a goofy goober, yeah\nWe're all goofy goobers, yeah\nGoofy, goofy, goober, goober, yeah (yeah)\nI'm a goofy goober, yeah\nYou're a goofy goober, yeah\nWe're all goofy goobers, yeah\nGoofy, goofy, goober, goober, yeah\n\nAnd here's your Triple Gooberberry Sunrise, sir\nOoh\nOh, Triple Gooberberry Sunrise, huh?\nI guess I could use one of those\nThere you go\n\nBoy, Pat, that hit the spot\nI'm feeling better already\nYeah\nWaiter, let's get another round over here\n\nOh, I'm a goofy goober, yeah\nYou're a goofy goober, yeah\nWe're all goofy goobers, yeah\nGoofy, goofy, goober, goober, yeah (yeah)\nI'm a goofy goober, yeah\nYou're a goofy goober, yeah\nWe're all goofy goobers, yeah\nGoofy, goofy, goober, goober, yeah (yeah)";
#include "Objects.h"
#include "OpenGL.h"
#include "Shapes.h"
#include "Input.h"
#include "GlobalVars.h"
#include "flowControl.h"
#include "ObjScripts.h"
#include "MathConstants.h"
#include "GlobalScripts.h"
#include <iostream>

struct changeColorButton : public ButtonMain {
	Vec4 color = 0;
	bool click = false;
	void onPressed() override {
		if (click)
			color = Vec4(1, 1, 0, 1);
		else
			color = Vec4(1, 0, 1, 1);
		click = !click;
	}
	void onHeld() override {
		thisObj->color = color;
	}
	void onReleased() override {
		thisObj->color = Vec4(1, 1, 1, 1);
	}
};


static Object* dropDownFieldObj = nullptr;
static Object* square1 = nullptr;
static Object* square2 = nullptr;
static Object* textFieldObj = nullptr;

static TextField* textFieldScr = nullptr;
DropDownFieldStatic* dropDownFieldScr = nullptr;


void progStart() {
	setBgColor(0.2, 0.3, 0.3);

	dropDownFieldObj = createObj("square");
	dropDownFieldObj->transform.scale = Vec3(0.4, 0.15, 1);
	dropDownFieldObj->transform.position = Vec3(_screenRatio - 0.4, 0.85, 0);
	dropDownFieldObj->color = Vec4(0.5, 0.5, 0.5, 1);

	dropDownFieldScr = new DropDownFieldStatic;
	dropDownFieldScr->buttonText.font = "CascadiaCode_NORMAL";
	dropDownFieldScr->buttonText.mode = TEXT_CENTER_RENDER;
	dropDownFieldScr->buttonText.fontSize = 0.5;
	dropDownFieldScr->buttonText.lineSize = 0.32;
	dropDownFieldScr->buttonText.fontColor = Vec4(0, 0, 0, 1);


	addObjScript(dropDownFieldObj, (void*)dropDownFieldScr, dropDownFieldScr->name);

    dropDownFieldScr->addOption("hey");

	square1 = createObj("square");
	square2 = createObj("square");

	square1->transform.scale = { 0.2, 0.2 };
	square1->transform.position.x = -0.7;
	square1->setDepth(-0.1f);

	square2->transform.scale = { 0.1, 0.1 };
	square2->setDepth(0.06f);
	square2->setParent(square1);

	square1->color = { 0, 1, 0, 1 };
	square2->color = { 1, 0, 0, 1 };
	square2->weak = true;


	textFieldObj = createObj("square");
	textFieldScr = new TextField();
	textFieldObj->transform.scale = { 1.5, 0.3 };
	textFieldObj->transform.position = { -_screenRatio + 1.5f, -0.7 };
    addObjScript(textFieldObj, (void*)textFieldScr, textFieldScr->name);

	textFieldScr->textScr->fontSize = 0.2;
	textFieldScr->textScr->lineSize = 0.21;
	textFieldScr->textScr->font = "CascadiaCode_NORMAL";
}


//double timeCount = 0;
//bool first = true;

void progMain() {
	if (keyAction::keyHeld(GLFW_KEY_LEFT))
        square2->relativeTransform.position.x -= 0.4f * _deltaTime;
	if (keyAction::keyHeld(GLFW_KEY_RIGHT))
        square2->relativeTransform.position.x += 0.4f * _deltaTime;
	if (keyAction::keyHeld(GLFW_KEY_UP))
        square2->relativeTransform.position.y += 0.4f * _deltaTime;
	if (keyAction::keyHeld(GLFW_KEY_DOWN))
        square2->relativeTransform.position.y -= 0.4f * _deltaTime;
	if (keyAction::keyHeld(GLFW_KEY_PAGE_UP))
		square1->transform.scale += 0.1f * _deltaTime;
	if (keyAction::keyHeld(GLFW_KEY_PAGE_DOWN))
		square1->transform.scale -= 0.1f * _deltaTime;
	if (keyAction::keyHeld(GLFW_KEY_KP_8) && keyAction::getNumlock())
		square1->transform.rotation += 20 * _deltaTime;
	if (keyAction::keyHeld(GLFW_KEY_KP_2) && keyAction::getNumlock())
		square1->transform.rotation -= 20 * _deltaTime;
	
	if (keyAction::keyPressed(GLFW_KEY_SPACE))
		dropDownFieldScr->addOption("another one");
    if (keyAction::keyPressed(GLFW_KEY_J)) {
        ((VoidButton*)((ObjectBase*)dropDownFieldScr->optionObjs[3].first)->scripts[0])->textScr->textUpdate();
    }



	//timeCount += _deltaTime;

	//if (timeCount >= 2) {
	//	if (!first) {
	//		for (uint i = 0; i < 5000; i++) {
	//			deleteObj(i);
	//		}
	//	}
	//	for (uint i = 0; i < 5000; i++) {
	//		createObj("square");
	//	}
	//	timeCount = 0;
	//	first = false;
	//}
}

void progEnd() {
}