#pragma once
#include "typeDefs.h"


struct buttonState {
	bool pressed = false;
	bool held = false;
	bool released = false;

	buttonState() : pressed(false), held(false), released(false) {}
};

namespace giveKeyAction {
	void keyPressed(int key);
	void keyReleased(int key);
	void latchSet();
	void scrolledUp();
	void scrolledDown();
	void setNumlock(bool val);
}

namespace keyAction {
	bool keyPressed(int key);
	bool keyHeld(int key);
	bool keyReleased(int key);
	bool scrolling();
	bool scrollUp();
	bool scrollDown();
	bool numlock();
}