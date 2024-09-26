#define _FLOW_CONTROL
#include "flowControl.h"
void progStart();
void progMain();
void progEnd();

Vec3 bgColor = Vec3(0);

static uint VAO;
static Shader myShader;

glm::mat4 transform(1);
glm::mat4 view(1);
glm::mat4 projection(1);


static ObjectBase* globalObjects[_MAX_OBJECTS];
static uint objCount = 0;

static uint myText = 0;

void start() {
	fontStart();
	for (uint i = 0; i < _MAX_OBJECTS; i++) {
		globalObjects[i] = nullptr;
	}

	myShader.createShader("./shaders/TVertexShader.vert", "./shaders/TFragmentShader.frag");

	projection = glm::ortho(-_screenRatio, _screenRatio, -1.0f, 1.0f, -1.0f, 1.0f);

	progStart();

	if (!loadFont("./fonts/font.ttf", "font")) {
		std::cout << "Failed to load fonts\n";
	}

	createTextTexture(myText,"font_LARGE", "a", 0.3, 1, 1, TEXT_LEFT_RENDER);
}

void update() {

	progMain();

	glClearColor(*bgColor.r, *bgColor.g, *bgColor.b, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	drawAllObjs();

	giveKeyAction::latchSet();
}

void end() {
	progEnd();
	deleteAll();
	deleteObjMapping();
	fontEnd();
}


//-------------------Functions

void setBgColor(Vec3 color) {
	bgColor = color;
}
void setBgColor(float r, float g, float b) {
	bgColor = Vec3(r, g, b);
}


//---------------------OBJECT----HANDLING

void drawAllObjs() {
	myShader.use();

	myShader.setMat4("view", glm::value_ptr(view));
	myShader.setMat4("projection", glm::value_ptr(projection));

	uint objDrawn = 0;

	for (int i = 0; i < _MAX_OBJECTS; i++) {
		if (objDrawn >= objCount)
			return;
		if (globalObjects[i] == nullptr)
			continue;

		Object* obj = ((Object*)globalObjects[i]);

		updateObjScripts(obj);

		transform = glm::mat4(1);
		transform = glm::translate(transform, glm::vec3(obj->position.toGLM(), 1));
		transform = glm::rotate(transform, (float)degToRad * obj->rotation, glm::vec3(0, 0, 1));
		transform = glm::scale(transform, glm::vec3(obj->scale.toGLM(), 1));

		myShader.setMat4("transform", glm::value_ptr(transform));
		myShader.setInt("texTarget", 0);
		myShader.setVec3("color", 1, 0, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, myText);



		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, *globalObjects[i]->VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *globalObjects[i]->EBO);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glDrawElements(GL_TRIANGLES, *globalObjects[i]->triCount, GL_UNSIGNED_INT, 0);
		objDrawn++;
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

uint findObjSlot() {
	for (uint i = 0; i < _MAX_OBJECTS; i++) {
		if (globalObjects[i] == nullptr)
			return i;
	}
	return _MAX_OBJECTS;
}

bool addGlobalObj(ObjectBase*& obj) {
	uint objSlot = findObjSlot();
	if (objSlot == _MAX_OBJECTS)
		return false;
	globalObjects[objSlot] = obj;
	globalObjects[objSlot]->index = objSlot;
	objCount++;
	return true;
}

void deleteAll() {
	for (uint i = 0; i < _MAX_OBJECTS; i++) {
		if (globalObjects[i] == nullptr)
			continue;
		clearObjScripts((Object*&)globalObjects[i]);
		delete(globalObjects[i]);
		globalObjects[i] = nullptr;
	}
	objCount = 0;
}

void deleteObj(uint index) {
	delete(globalObjects[index]);
	clearObjScripts((Object*&)globalObjects[index]);
	globalObjects[index] = nullptr;
	objCount--;
}

void deleteObj(Object*& obj) {
	uint index = obj->getIndex();
	clearObjScripts(obj);
	delete(globalObjects[index]);
	globalObjects[index] = nullptr;
	objCount--;
}


//-------------------CALLBACKS

void windowScaled(GLFWwindow* window, int width, int height) {
	_Width = width;
	_Height = height;
	_screenRatio = (float)_Width / _Height;
	glViewport(0, 0, width, height);
}
void mouseMove(GLFWwindow* window, double xpos, double ypos) {
	mousePosX = xpos / _Height * 2 - _screenRatio;
	mousePosY = (_Height - ypos) / _Height * 2 - 1;
}
void keyPress(GLFWwindow* window, int button, int scancode, int action, int mods) {
	if (action == GLFW_PRESS)
		giveKeyAction::keyPressed(button);
	else if (action == GLFW_RELEASE)
		giveKeyAction::keyReleased(button);
}
void mouseScroll(GLFWwindow* window, double xoff, double yoff) {
	if (yoff > 0)
		giveKeyAction::scrolledUp();
	else if (yoff < 0)
		giveKeyAction::scrolledDown();
}
void mouseClick(GLFWwindow* window, int button, int action, int mods) {
	if (action == GLFW_PRESS)
		giveKeyAction::keyPressed(button);
	else if (action == GLFW_RELEASE)
		giveKeyAction::keyReleased(button);
}



//--------------------FOR-OBJECTS

//Adds a script to object
void addObjScript(Object*& obj, void* script) {
	((ObjectBase*)obj)->scripts.push_back(script);
	scriptBase* realScr = (scriptBase*)script;
	realScr->thisObj = (Object*)obj;
	realScr->start();
}

//Removes a specific script from object
void removeObjScript(Object*& obj, unsigned int index) {
	delete(((ObjectBase*)obj)->scripts[index]);
	((ObjectBase*)obj)->scripts.erase(((ObjectBase*)obj)->scripts.begin() + index);
}

//Returns the index of a script in the list on an object
unsigned int getObjScriptIndex(Object*& obj, std::string name) {
	for (uint i = 0; i < ((ObjectBase*)obj)->scripts.size(); i++) {
		scriptBase* realScr = (scriptBase*)((ObjectBase*)obj)->scripts[i];
		if (realScr->name == name) {
			return i;
		}
	}
	return UINT_MAX;
}

//Calls the update function on all scripts for an object
void updateObjScripts(Object*& obj) {
	for (uint i = 0; i < ((ObjectBase*)obj)->scripts.size(); i++) {
		scriptBase* realScr = (scriptBase*)((ObjectBase*)obj)->scripts[i];
		realScr->update();
	}
}

//Clears all scripts from an object
void clearObjScripts(Object*& obj) {
	for (uint i = 0; i < ((ObjectBase*)obj)->scripts.size(); i++) {
		delete(((ObjectBase*)obj)->scripts[i]);
	}
	((ObjectBase*)obj)->scripts.clear();
}