#include "Objects.h"
#include "OpenGL.h"
#include "Shapes.h"
#include "input.h"
#include "globalVars.h"
#include "flowControl.h"
#include "objScripts.h"
#include "MathConstants.h"

static uint SVBO, SEBO, StriCount;
static uint VBO, EBO, triCount;

static Object* obj = nullptr;

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

	VBO = SVBO;
	EBO = SEBO;
	triCount = StriCount;

	loadBufferObj("myObj", &VBO, &EBO, &triCount);

	obj = createObj("myObj");
	obj->scale.x = (float)16 / 9;
}

bool shape = false;

void progMain() {

}

void progEnd() {
	glDeleteBuffers(1, &SVBO);
	glDeleteBuffers(1, &SEBO);
}