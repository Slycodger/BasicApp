#include "ProgUI.h"
#include "globalVars.h"
#include "flowControl.h"
#include "globalScripts.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Object* button = nullptr;
Button* buttonScr = nullptr;
Vec2 offset;

const char* imageLoc = "textures/image.png";
unsigned int buttonTexture = 0;

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

    int iWidth, iHeight, iChannels = 0;

    glGenTextures(1, &buttonTexture);
    glBindTexture(GL_TEXTURE_2D, buttonTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);
    unsigned char* imageData = stbi_load(imageLoc, &iWidth, &iHeight, &iChannels, 0);
    if (imageData) {
        button->setTexture(buttonTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iWidth, iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
        glGenerateMipmap(buttonTexture);
        
        button->transform.scale.x = 0.5 * (float)iWidth / iHeight;
    }
    else {
        std::cout << "Failed to load texture\n";
    }
    stbi_image_free(imageData);

    buttonScr->BonPressed = onPressed;
    buttonScr->BonHeld = onHeld;


    addObjScript(button, (void*)buttonScr);

    button->color = { 0.9, 0.9, 0.9, 0.7 };
    buttonScr->textScr->fontColor = { 1, 1, 1, 1 };
    buttonScr->textScr->fontSize = 0.2;
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
    glDeleteTextures(1, &buttonTexture);
}