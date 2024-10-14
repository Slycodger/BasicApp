#define _INPUT
#include "input.h"

constexpr uint keyCount = 349;

static bool numlock = false;
static bool shift = false;

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
	void setNumlock(bool val) {
		numlock = val;
	}
	void setShift(bool val) {
		shift = val;
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
	bool getNumlock() {
		return numlock;
	}
	bool getShift() {
		return shift;
	}
	void buildText(std::string& text) {
		if (keyPressed(GLFW_KEY_BACKSPACE) && text.length())		text.pop_back();
		if (keyPressed(GLFW_KEY_SPACE))								text += ' ';
		if (keyPressed(GLFW_KEY_ENTER))								text += '\n';
		if (keyPressed(GLFW_KEY_TAB))								text += '\t';
		if (keyPressed(GLFW_KEY_A))									if (shift) text += 'A'; else text += 'a';
		if (keyPressed(GLFW_KEY_B))									if (shift) text += 'B'; else text += 'b';
		if (keyPressed(GLFW_KEY_C))									if (shift) text += 'C'; else text += 'c';
		if (keyPressed(GLFW_KEY_D))									if (shift) text += 'D'; else text += 'd';
		if (keyPressed(GLFW_KEY_E))									if (shift) text += 'E'; else text += 'e';
		if (keyPressed(GLFW_KEY_F))									if (shift) text += 'F'; else text += 'f';
		if (keyPressed(GLFW_KEY_G))									if (shift) text += 'G'; else text += 'g';
		if (keyPressed(GLFW_KEY_H))									if (shift) text += 'H'; else text += 'h';
		if (keyPressed(GLFW_KEY_I))									if (shift) text += 'I'; else text += 'i';
		if (keyPressed(GLFW_KEY_J))									if (shift) text += 'J'; else text += 'j';
		if (keyPressed(GLFW_KEY_K))									if (shift) text += 'K'; else text += 'k';
		if (keyPressed(GLFW_KEY_L))									if (shift) text += 'L'; else text += 'l';
		if (keyPressed(GLFW_KEY_M))									if (shift) text += 'M'; else text += 'm';
		if (keyPressed(GLFW_KEY_N))									if (shift) text += 'N'; else text += 'n';
		if (keyPressed(GLFW_KEY_O))									if (shift) text += 'O'; else text += 'o';
		if (keyPressed(GLFW_KEY_P))									if (shift) text += 'P'; else text += 'p';
		if (keyPressed(GLFW_KEY_Q))									if (shift) text += 'Q'; else text += 'q';
		if (keyPressed(GLFW_KEY_R))									if (shift) text += 'R'; else text += 'r';
		if (keyPressed(GLFW_KEY_S))									if (shift) text += 'S'; else text += 's';
		if (keyPressed(GLFW_KEY_T))									if (shift) text += 'T'; else text += 't';
		if (keyPressed(GLFW_KEY_U))									if (shift) text += 'U'; else text += 'u';
		if (keyPressed(GLFW_KEY_V))									if (shift) text += 'V'; else text += 'v';
		if (keyPressed(GLFW_KEY_W))									if (shift) text += 'W'; else text += 'w';
		if (keyPressed(GLFW_KEY_X))									if (shift) text += 'X'; else text += 'x';
		if (keyPressed(GLFW_KEY_Y))									if (shift) text += 'Y'; else text += 'y';
		if (keyPressed(GLFW_KEY_Z))									if (shift) text += 'Z'; else text += 'z';
		if (keyPressed(GLFW_KEY_APOSTROPHE))						if (shift) text += '\"'; else text += '\'';
		if (keyPressed(GLFW_KEY_COMMA))								if (shift) text += '<'; else text += ',';
		if (keyPressed(GLFW_KEY_MINUS))								if (shift) text += '_'; else text += '-';
		if (keyPressed(GLFW_KEY_PERIOD))							if (shift) text += '>'; else text += '.';
		if (keyPressed(GLFW_KEY_SLASH))								if (shift) text += '?'; else text += '/';
		if (keyPressed(GLFW_KEY_SEMICOLON))							if (shift) text += ':'; else text += ';';
		if (keyPressed(GLFW_KEY_EQUAL))								if (shift) text += '+'; else text += '=';
		if (keyPressed(GLFW_KEY_LEFT_BRACKET))						if (shift) text += '{'; else text += '[';
		if (keyPressed(GLFW_KEY_BACKSLASH))							if (shift) text += '|'; else text += '\\';
		if (keyPressed(GLFW_KEY_RIGHT_BRACKET))						if (shift) text += '}'; else text += ']';
		if (keyPressed(GLFW_KEY_GRAVE_ACCENT))						if (shift) text += '~'; else text += '`';
		if (keyPressed(GLFW_KEY_1))									if (shift) text += '!'; else text += '1';
		if (keyPressed(GLFW_KEY_2))									if (shift) text += '@'; else text += '2';
		if (keyPressed(GLFW_KEY_3))									if (shift) text += '#'; else text += '3';
		if (keyPressed(GLFW_KEY_4))									if (shift) text += '$'; else text += '4';
		if (keyPressed(GLFW_KEY_5))									if (shift) text += '%'; else text += '5';
		if (keyPressed(GLFW_KEY_6))									if (shift) text += '^'; else text += '6';
		if (keyPressed(GLFW_KEY_7))									if (shift) text += '&'; else text += '7';
		if (keyPressed(GLFW_KEY_8))									if (shift) text += '*'; else text += '8';
		if (keyPressed(GLFW_KEY_9))									if (shift) text += '('; else text += '9';
		if (keyPressed(GLFW_KEY_0))									if (shift) text += ')'; else text += '0';
	}
}