#include "ProgUI.h"
#include "globalVars.h"
#include "flowControl.h"
#include "globalScripts.h"

Object* button = nullptr;
Button* buttonScr = nullptr;
Vec2 offset;

void onPressed(Button* button) {
    offset = { button->thisObj->transform.position.x - _mousePosX, button->thisObj->transform.position.y - _mousePosY };
}

void onHeld(Button* button) {
    button->thisObj->transform.position = offset + Vec2(_mousePosX, _mousePosY);
}

void progStart() {
    setBgColor(0.2, 0.3, 0.3, 0);

    button = createObj("square");
    button->transform.scale = 0.5;
    buttonScr = new Button;

    buttonScr->BonPressed = onPressed;
    buttonScr->BonHeld = onHeld;



    addObjScript(button, (void*)buttonScr);

    button->color = { 0, 0, 0, 1 };
    buttonScr->textScr->fontColor = 0.5;
    buttonScr->textScr->text = "Drag to move";
    buttonScr->textScr->textUpdate();
}


void progMain() {
    if (keyAction::keyPressed(GLFW_KEY_SPACE))
        progUI::fps();
    if (keyAction::keyPressed(GLFW_KEY_A))
        _vsync = !_vsync;
}

void progEnd() {

}