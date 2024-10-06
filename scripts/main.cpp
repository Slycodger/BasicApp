#include "Objects.h"
#include "OpenGL.h"
#include "Shapes.h"
#include "input.h"
#include "globalVars.h"
#include "flowControl.h"
#include "objScripts.h"
#include "MathConstants.h"
#include "globalScripts.h"
#include <iostream>

static uint SVBO, SEBO, StriCount;
static uint VBO, EBO, triCount;

static Object* obj = nullptr;

static TextBox* textScr = new TextBox;


void progStart() {
	setBgColor(0.2, 0.3, 0.3);

	createBufferObj(SVBO, SEBO, squareVertices, squareIndices, sizeof(squareVertices), sizeof(squareIndices));
	StriCount = 6;

	VBO = SVBO;
	EBO = SEBO;
	triCount = StriCount;

	loadBufferObj("myObj", &VBO, &EBO, &triCount);

	if (!loadFont("./fonts/CascadiaCode.ttf", "CascadiaCode")) {
		std::cout << "Failed to load font\n";
	}

	obj = createObj("myObj");
	obj->scale.x = 0.8f;
	textScr->text = "Hey, all you Goobers\nIt's time to say howdy to your favorite undersea peanut, Goofy Goober\n(Yeah)\n(Yeah)\n(Yeah)\n\nAlright, folks, this one goes out to my two bestest friends in the whole world\nPatrick and this big peanut guy\nIt's a little ditty called\nGoofy Goober (yeah)\n\nOh, I'm a goofy goober, yeah\nYou're a goofy goober, yeah\nWe're all goofy goobers, yeah\nGoofy, goofy, goober, goober, yeah (yeah)\nI'm a goofy goober, yeah\nYou're a goofy goober, yeah\nWe're all goofy goobers, yeah\nGoofy, goofy, goober, goober, yeah (yeah)\n\nDJ (yeah), time for the test (yeah)\nNo baby can resist singin' along to this (yeah)\n(Yeah, yeah)\n(Yeah, yeah)\n(Yeah, yeah)\n\nSpongeBob, it's the Goofy Goober theme song\nI know\n\nOh, I'm a goofy goober, yeah\nYou're a goofy goober, yeah\nWe're all goofy goobers, yeah\nGoofy, goofy, goober, goober, yeah (yeah)\nI'm a goofy goober, yeah\nYou're a goofy goober, yeah\nWe're all goofy goobers, yeah\nGoofy, goofy, goober, goober, yeah\n\nAnd here's your Triple Gooberberry Sunrise, sir\nOoh\nOh, Triple Gooberberry Sunrise, huh?\nI guess I could use one of those\nThere you go\n\nBoy, Pat, that hit the spot\nI'm feeling better already\nYeah\nWaiter, let's get another round over here\n\nOh, I'm a goofy goober, yeah\nYou're a goofy goober, yeah\nWe're all goofy goobers, yeah\nGoofy, goofy, goober, goober, yeah (yeah)\nI'm a goofy goober, yeah\nYou're a goofy goober, yeah\nWe're all goofy goobers, yeah\nGoofy, goofy, goober, goober, yeah (yeah)";
	textScr->font = "CascadiaCode_SMALL";
	textScr->fontSize = 0.03f;
	textScr->lineSize = 0.034f;
	textScr->mode = TEXT_LEFT_RENDER;
	textScr->fontColor = Vec4(1, 1, 0, 1);

	addObjScript(obj, (void*)textScr);

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


}

void progEnd() {
	glDeleteBuffers(1, &SVBO);
	glDeleteBuffers(1, &SEBO);
}