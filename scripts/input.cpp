#include "input.h"

constexpr uint keyCount = 349;

buttonState allButtons[keyCount];
bool scrollingUp = false;
bool scrollingDown = false;

namespace giveKeyAction {
	void keyPressed(int key) {
		allButtons[key].pressed = true;
		allButtons[key].held = true;
		allButtons[key].released = false;
	}
	void keyReleased(int key) {
		allButtons[key].pressed = false;
		allButtons[key].held = false;
		allButtons[key].released = true;
	}
	void latchSet() {
		for (int i = 0; i < keyCount; i++) {
			allButtons[i].pressed = false;
			allButtons[i].released = false;
		}
		scrollingUp = false;
		scrollingDown = false;
	}
	void scrolledUp() {
		scrollingUp = true;
		scrollingDown = false;
	}
	void scrolledDown() {
		scrollingUp = false;
		scrollingDown = true;
	}
}

namespace keyAction {
	bool keyPressed(int key) {
		return allButtons[key].pressed;
	}
	bool keyHeld(int key) {
		return allButtons[key].held;
	}
	bool keyReleased(int key) {
		return allButtons[key].released;
	}
	bool scrolling() {
		return scrollingUp || scrollingDown;
	}
	bool scrollUp() {
		return scrollingUp;
	}
	bool scrollDown() {
		return scrollingDown;
	}
}