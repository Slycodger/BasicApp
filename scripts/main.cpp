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

static Object* obj = nullptr;
static Object* square1 = nullptr;
static Object* square2 = nullptr;

static TextBox* textScr = nullptr;
static Button* buttonScr = new Button;

static const char* const sponge = "Hey, all you Goobers\nIt's time to say howdy to your favorite undersea peanut, Goofy Goober\n(Yeah)\n(Yeah)\n(Yeah)\n\nAlright, folks, this one goes out to my two bestest friends in the whole world\nPatrick and this big peanut guy\nIt's a little ditty called\nGoofy Goober (yeah)\n\nOh, I'm a goofy goober, yeah\nYou're a goofy goober, yeah\nWe're all goofy goobers, yeah\nGoofy, goofy, goober, goober, yeah (yeah)\nI'm a goofy goober, yeah\nYou're a goofy goober, yeah\nWe're all goofy goobers, yeah\nGoofy, goofy, goober, goober, yeah (yeah)\n\nDJ (yeah), time for the test (yeah)\nNo baby can resist singin' along to this (yeah)\n(Yeah, yeah)\n(Yeah, yeah)\n(Yeah, yeah)\n\nSpongeBob, it's the Goofy Goober theme song\nI know\n\nOh, I'm a goofy goober, yeah\nYou're a goofy goober, yeah\nWe're all goofy goobers, yeah\nGoofy, goofy, goober, goober, yeah (yeah)\nI'm a goofy goober, yeah\nYou're a goofy goober, yeah\nWe're all goofy goobers, yeah\nGoofy, goofy, goober, goober, yeah\n\nAnd here's your Triple Gooberberry Sunrise, sir\nOoh\nOh, Triple Gooberberry Sunrise, huh?\nI guess I could use one of those\nThere you go\n\nBoy, Pat, that hit the spot\nI'm feeling better already\nYeah\nWaiter, let's get another round over here\n\nOh, I'm a goofy goober, yeah\nYou're a goofy goober, yeah\nWe're all goofy goobers, yeah\nGoofy, goofy, goober, goober, yeah (yeah)\nI'm a goofy goober, yeah\nYou're a goofy goober, yeah\nWe're all goofy goobers, yeah\nGoofy, goofy, goober, goober, yeah (yeah)";


void progStart() {
	setBgColor(0.2, 0.3, 0.3);

	if (!loadFont("./fonts/CascadiaCode.ttf", "CascadiaCode")) {
		std::cout << "Failed to load font\n";
	}

	obj = createObj("square");
	obj->transform.scale = Vec3(0.4, 0.3, 1);
	obj->transform.position = Vec3(_screenRatio - 0.4, -0.7, 0);


	textScr = buttonScr->textScr;
	textScr->text = "button";
	textScr->font = "CascadiaCode_SMALL";
	textScr->fontSize = 0.6;
	textScr->lineSize = 1;
	textScr->mode = TEXT_LEFT_RENDER;
	textScr->fontColor = Vec4(1, 1, 0, 1);

	addObjScript(obj, (void*)buttonScr);

	square1 = createObj("square");
	square2 = createObj("square");

	square1->transform.scale = { 0.2, 0.2, 1 };
	square1->transform.position.x = -0.7;

	square2->transform.scale = { 0.1, 0.1, 1 };
	square2->setParent(square1);

	square1->color = { 0, 1, 0, 1 };
	square2->color = { 1, 0, 0, 1 };

}

bool shape = false;

void progMain() {
	if (keyAction::scrollUp()) {
		textScr->fontSize += 0.05f;
		textScr->lineSize += 0.05f;
	}
	if (keyAction::scrollDown()) {
		textScr->fontSize -= 0.05f;
		textScr->lineSize -= 0.05f;
	}
	if (keyAction::keyPressed(GLFW_KEY_SPACE)) {
		if (textScr->mode == TEXT_LEFT_RENDER)
			textScr->mode = TEXT_CENTER_RENDER;
		else
			textScr->mode = TEXT_LEFT_RENDER;
	}

	if (keyAction::keyPressed(GLFW_KEY_LEFT))
		square1->transform.position.x -= 0.05f;
	if (keyAction::keyPressed(GLFW_KEY_RIGHT))
		square1->transform.position.x += 0.05f;
	if (keyAction::keyPressed(GLFW_KEY_UP))
		square1->transform.position.y += 0.05f;
	if (keyAction::keyPressed(GLFW_KEY_DOWN))
		square1->transform.position.y -= 0.05f;
	if (keyAction::keyPressed(GLFW_KEY_PAGE_UP))
		square1->transform.scale += 0.05f;
	if (keyAction::keyPressed(GLFW_KEY_PAGE_DOWN))
		square1->transform.scale -= 0.05f;
	if (keyAction::keyPressed(GLFW_KEY_KP_8) && keyAction::numlock())
		square1->transform.rotation.z += 5;
	if (keyAction::keyPressed(GLFW_KEY_KP_2) && keyAction::numlock())
		square1->transform.rotation.z -= 5;


}

void progEnd() {
}