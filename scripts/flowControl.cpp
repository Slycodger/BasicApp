#define _FLOW_CONTROL
#include "flowControl.h"
void progStart();
void progMain();
void progEnd();

Vec3 bgColor = Vec3(0);

static uint VAO;
static Shader shader;

glm::mat4 transform(1);
glm::mat4 view(1);
glm::mat4 projection(1);

bool windowScaled = false;


static ObjectBase* globalObjects[_MAX_OBJECTS];
static uint objCount = 0;

static uint myText = 0;

void start() {
	Text::start();
	Objects::start();
	for (uint i = 0; i < _MAX_OBJECTS; i++) {
		globalObjects[i] = nullptr;
	}

	addShader("noTextureShader", "./shaders/NTVertexShader.vert", "./shaders/NTFragmentShader.frag");
	addShader("textureShader", "./shaders/TVertexShader.vert", "./shaders/TFragmentShader.frag");

	projection = glm::ortho(-_screenRatio, _screenRatio, -1.0f, 1.0f, -1.0f, 1.0f);

	progStart();
}

void update() {

	progMain();

	glClearColor(bgColor.x, bgColor.y, bgColor.z, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawAllObjs();

	windowScaled = false;

	giveKeyAction::latchSet();
}

void end() {
	progEnd();
	deleteAll();
	deleteObjMapping();

	Text::end();
	Objects::end();
	Shaders::end();
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
	uint objDrawn = 0;

	for (int i = 0; i < _MAX_OBJECTS; i++) {
		if (objDrawn >= objCount)
			return;
		if (globalObjects[i] == nullptr)
			continue;

		Object* obj = ((Object*)globalObjects[i]);

		updateObjScripts(obj);
		if (obj->parent == nullptr)
			updateObjChildren(obj);

		transform = glm::mat4(1);
		transform = glm::translate(transform, glm::vec3(obj->transform.position.toGLM(), obj->depth));
		transform = glm::rotate(transform, (float)degToRad * obj->transform.rotation, glm::vec3(0, 0, 1));
		transform = glm::scale(transform, glm::vec3(obj->transform.scale.toGLM(), 1));
		if (obj->usesTexture()) {
			uint texTarget = obj->getTexture();
			shader.use(getShader("textureShader"));
			shader.setMat4("transform", glm::value_ptr(transform));
			shader.setMat4("view", glm::value_ptr(view));
			shader.setMat4("projection", glm::value_ptr(projection));
			shader.setVec2("screenSize", _Width, _Height);
			shader.setVec4("color", obj->color);
			shader.setInt("texture", 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texTarget);
		}
		else {
			shader.use(getShader("noTextureShader"));
			shader.setMat4("transform", glm::value_ptr(transform));
			shader.setMat4("view", glm::value_ptr(view));
			shader.setMat4("projection", glm::value_ptr(projection));
			shader.setVec4("color", obj->color);
		}

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

void windowScaleCallback(GLFWwindow* window, int width, int height) {
	_Width = width;
	_Height = height;
	_screenRatio = (float)_Width / _Height;
	glViewport(0, 0, width, height);
	projection = glm::ortho(-_screenRatio, _screenRatio, -1.0f, 1.0f, -1.0f, 1.0f);
	windowScaled = true;
}
void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos) {
	mousePosX = xpos / _Height * 2 - _screenRatio;
	mousePosY = (_Height - ypos) / _Height * 2 - 1;
}
void keyPressCallback(GLFWwindow* window, int button, int scancode, int action, int mods) {
	if (action == GLFW_PRESS)
		giveKeyAction::keyPressed(button);
	else if (action == GLFW_RELEASE)
		giveKeyAction::keyReleased(button);
	giveKeyAction::setNumlock(mods & GLFW_MOD_NUM_LOCK);
}
void mouseScrollCallback(GLFWwindow* window, double xoff, double yoff) {
	if (yoff > 0)
		giveKeyAction::scrolledUp();
	else if (yoff < 0)
		giveKeyAction::scrolledDown();
}
void mouseClickCallback(GLFWwindow* window, int button, int action, int mods) {
	if (action == GLFW_PRESS)
		giveKeyAction::keyPressed(button);
	else if (action == GLFW_RELEASE)
		giveKeyAction::keyReleased(button);
}



//--------------------FOR-OBJECTS

/// <summary>
/// 
/// </summary>
void addObjScript(Object*& obj, void* script, std::string scrName) {
	((ObjectBase*)obj)->scripts.push_back(script);
	scriptBase* realScr = (scriptBase*)script;
	realScr->thisObj = obj;
	realScr->name = scrName;
	realScr->start();
}

/// <summary>
/// Remove an object script at a specific index
/// </summary>
void removeObjScript(Object*& obj, unsigned int index) {
	((scriptBase*)((ObjectBase*)obj)->scripts[index])->end();
	delete(((ObjectBase*)obj)->scripts[index]);
	((ObjectBase*)obj)->scripts.erase(((ObjectBase*)obj)->scripts.begin() + index);
}

/// <summary>
/// Get the index of a specific script on an object
/// </summary>
/// <returns>An unsigned int with the index</returns>
unsigned int getObjScriptIndex(Object*& obj, std::string name) {
	for (uint i = 0; i < ((ObjectBase*)obj)->scripts.size(); i++) {
		scriptBase* realScr = (scriptBase*)((ObjectBase*)obj)->scripts[i];
		if (realScr->name == name) {
			return i;
		}
	}
	return UINT_MAX;
}

/// <summary>
/// Get an object script from name
/// </summary>
/// <returns>A void* to the script, you have to cast it yourself</returns>
void* getObjScript(Object*& obj, std::string name) {
	uint index = getObjScriptIndex(obj, name);
	if (index >= UINT_MAX)
		return nullptr;
	return ((ObjectBase*)obj)->scripts[index];
}

/// <summary>
/// Updates all of an objects scripts
/// </summary>
void updateObjScripts(Object*& obj) {
	for (uint i = 0; i < ((ObjectBase*)obj)->scripts.size(); i++) {
		scriptBase* realScr = (scriptBase*)((ObjectBase*)obj)->scripts[i];
		realScr->update();
	}
}

/// <summary>
/// Deletes all the scripts from an object
/// </summary>
void clearObjScripts(Object*& obj) {
	for (uint i = 0; i < ((ObjectBase*)obj)->scripts.size(); i++) {
		((scriptBase*)((ObjectBase*)obj)->scripts[i])->end();
		delete(((ObjectBase*)obj)->scripts[i]);
	}
	((ObjectBase*)obj)->scripts.clear();
}

void updateObjChildren(Object*& obj) {
	if (obj->children.size() == 0)
		return;
	for (Object* child : obj->children) {
		child->setToRelative();
		updateObjChildren(child);
	}
}