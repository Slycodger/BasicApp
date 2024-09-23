#include "Objects.h"
#include "OpenGL.h"
#include "Shapes.h"
#include "input.h"
#include "globalVars.h"
#include "flowControl.h"
#include "objScripts.h"
#include "MathConstants.h"

uint SVBO, SEBO, StriCount;
uint TVBO, TEBO, TtriCount;
uint VBO, EBO, triCount;

Object* obj = nullptr;

struct tempScript : public scriptBase {
	bool color = true;
	uint time = 0;
	void start() override {
		thisObj->scale = Vec2(1.5, 0.25);
		thisObj->color = Vec3(0, 1, 0);
	}
	void update() override {
		float left = thisObj->position.x - thisObj->scale.x;
		float right = thisObj->position.x + thisObj->scale.x;
		float bottom = thisObj->position.y - thisObj->scale.y;
		float top = thisObj->position.y + thisObj->scale.y;
		if (keyAction::keyPressed(GLFW_MOUSE_BUTTON_1)) {
			if (mousePosX <= right && mousePosX >= left && mousePosY <= top && mousePosY >= bottom) {
				if (color) {
					thisObj->color = Vec3(1, 0, 0);
					color = false;
				}
				else {
					thisObj->color = Vec3(0, 1, 0);
					color = true;
				}
			}
		}

		thisObj->position.y = 0.75 * sin(degToRad * time);

		time++;
	}
};

void progStart() {
	setBgColor(0.2, 0.3, 0.3);

	createBufferObj(SVBO, SEBO, squareVertices, squareIndices, sizeof(squareVertices), sizeof(squareIndices));
	StriCount = 6;

	createBufferObj(TVBO, TEBO, triangleVertices, triangleIndices, sizeof(triangleVertices), sizeof(triangleIndices));
	TtriCount = 3;

	VBO = SVBO;
	EBO = SEBO;
	triCount = StriCount;

	loadBufferObj("myObj", &VBO, &EBO, &triCount);

	obj = createObj("myObj");
	addObjScript(obj, (void*)(new tempScript));
}

bool shape = false;

void progMain() {
	if (keyAction::keyPressed(GLFW_KEY_SPACE)) {
		if (!shape) {
			VBO = TVBO;
			EBO = TEBO;
			triCount = TtriCount;
			shape = true;
		} else {
			VBO = SVBO;
			EBO = SEBO;
			triCount = StriCount;
			shape = false;
		}
	}
}

void progEnd() {
	glDeleteBuffers(1, &SVBO);
	glDeleteBuffers(1, &SEBO);
	glDeleteBuffers(1, &TVBO);
	glDeleteBuffers(1, &TEBO);
}