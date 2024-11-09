#define _FLOW_CONTROL
#include "flowControl.h"
void progStart();
void progMain();
void progEnd();

//App globals

Vec4 _bgColor = Vec4(0);
iVec2 _windowPos = iVec2(0);
iVec2 _realMousePos = iVec2(0);

glm::mat4 _transform(1);
glm::mat4 _view(1);
glm::mat4 _projection(1);

bool _windowScaled = false;
bool _lockMouse = false;

float _mousePosX = 0;
float _mousePosY = 0;


//Local globals

static uint VAO;
static Shader shader;

static std::vector<ObjectBase*> globalObjects(_MAX_OBJECTS);
static uint objCount = 0;

void start() {
	for (uint i = 0; i < _MAX_OBJECTS; i++) {
		globalObjects[i] = nullptr;
	}

    Text::start();
    Objects::start();
    ProgUI::start();


	addShader("noTextureShader", "./shaders/NTVertexShader.vert", "./shaders/NTFragmentShader.frag");
	addShader("textureShader", "./shaders/TVertexShader.vert", "./shaders/TFragmentShader.frag");

	_projection = glm::ortho(-_screenRatio, _screenRatio, -1.0f, 1.0f, -1.0f, 1.0f);

	progStart();
}

void update() {
    progMain();
    ProgUI::update();

	glClearColor(_bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    drawAllObjs();
	_windowScaled = false;

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

void setBgColor(Vec4 color) {
	_bgColor = color;
}
void setBgColor(float r, float g, float b, float a) {
	_bgColor = Vec4(r, g, b, a);
}


//---------------------OBJECT----HANDLING

void drawAllObjs() {
	if (objCount == 0)
		return;

	uint objDrawn = 0;
	std::sort(globalObjects.begin(), globalObjects.end(), objCmp);
    Object* prevDependent = nullptr;

	for (int i = 0; i < _MAX_OBJECTS; i++) {
		if (objDrawn >= objCount)
			break;
        if (globalObjects[i] == nullptr)
            continue;

		ObjectBase*& objBase = globalObjects[i];
		Object*& obj = (Object*&)objBase;

		if (!Object::chainActive(obj)) {
			objDrawn++;
			continue;
		}

        if (obj->parent == nullptr)
            updateObjChildren(obj);
        updateObjScripts(obj);

        if (obj->dependent != nullptr) {
            glEnable(GL_STENCIL_TEST);
            if (obj->dependent != prevDependent) {
                glClear(GL_STENCIL_BUFFER_BIT);
                drawObjStencil(obj->dependent);
            }
            prevDependent = obj->dependent;
            glStencilFunc(GL_EQUAL, 0x01, 0xFF);
            glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        }
        else
            glDisable(GL_STENCIL_TEST);

		if (obj->weak) {
			glBlendEquation(GL_FUNC_ADD);
			glBlendFuncSeparate(GL_ONE_MINUS_DST_ALPHA, GL_DST_ALPHA, GL_ZERO, GL_ONE);
		}
		else {
			glBlendEquation(GL_FUNC_ADD);
			glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
		}

		_transform = glm::mat4(1);
		_transform = glm::translate(_transform, glm::vec3(obj->transform.position.toGLM(), objBase->depth));
		_transform = glm::rotate(_transform, (float)degToRad * obj->transform.rotation, glm::vec3(0, 0, 1));
		_transform = glm::scale(_transform, glm::vec3(obj->transform.scale.toGLM(), 1));
		if (obj->usesTexture()) {
			uint texTarget = obj->getTexture();
			shader.use(getShader("textureShader"));
			shader.setMat4("transform", _transform);
			shader.setMat4("view", _view);
			shader.setMat4("projection", _projection);
			shader.setVec4("color", obj->color);
			shader.setInt("texTarget", 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texTarget);
		}
		else {
			shader.use(getShader("noTextureShader"));
			shader.setMat4("transform", _transform);
			shader.setMat4("view", _view);
			shader.setMat4("projection", _projection);
			shader.setVec4("color", obj->color);
		}

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, *objBase->VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *objBase->EBO);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glDrawElements(GL_TRIANGLES, *objBase->triCount, GL_UNSIGNED_INT, 0);
		objDrawn++;
	}
    glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void drawObjStencil(Object* obj) {
    glStencilFunc(GL_ALWAYS, 0x01, 0xFF);
    glStencilOp(GL_INCR, GL_INCR, GL_INCR);
    glDisable(GL_DEPTH_TEST);
    glColorMask(0, 0, 0, 0);

    ObjectBase*& objBase = (ObjectBase*&)obj;

    _transform = glm::mat4(1);
    _transform = glm::translate(_transform, glm::vec3(obj->transform.position.toGLM(), objBase->depth));
    _transform = glm::rotate(_transform, (float)degToRad * obj->transform.rotation, glm::vec3(0, 0, 1));
    _transform = glm::scale(_transform, glm::vec3(obj->transform.scale.toGLM(), 1));

    shader.use(getShader("noTextureShader"));
    shader.setMat4("transform", _transform);
    shader.setMat4("view", _view);
    shader.setMat4("projection", _projection);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, *objBase->VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *objBase->EBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glDrawElements(GL_TRIANGLES, *objBase->triCount, GL_UNSIGNED_INT, 0);

    glEnable(GL_DEPTH_TEST);
    glColorMask(1, 1, 1, 1);
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
		clearObjChildren((Object*&)globalObjects[i]);
		delete(globalObjects[i]);
		globalObjects[i] = nullptr;
	}
	objCount = 0;
	globalObjects.clear();
	globalObjects.~vector();
}

void deleteObj(uint index) {
	if (globalObjects[index] == nullptr)
		return;
	clearObjScripts((Object*&)globalObjects[index]);
	clearObjChildren((Object*&)globalObjects[index]);
	delete(globalObjects[index]);
	globalObjects[index] = nullptr;
	objCount--;
}

void deleteObj(Object*& obj) {
	if (obj == nullptr)
		return;
	uint index = obj->getIndex();
	clearObjScripts(obj);
	clearObjChildren(obj);
	delete(globalObjects[index]);
	globalObjects[index] = nullptr;
	objCount--;
	obj = nullptr;
}


//-------------------CALLBACKS

void windowScaleCallback(GLFWwindow* window, int width, int height) {
	_Width = width;
	_Height = height;
	_screenRatio = (float)_Width / _Height;
	glViewport(0, 0, width, height);
	_projection = glm::ortho(-_screenRatio, _screenRatio, -1.0f, 1.0f, -1.0f, 1.0f);
	_windowScaled = true;
}
void windowMoveCallback(GLFWwindow* window, int xpos, int ypos) {
	_windowPos.x = xpos;
	_windowPos.y = ypos;
}
void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos) {
	if (_lockMouse) {
		SetCursorPos(_windowPos.x + _realMousePos.x, _windowPos.y + _realMousePos.y);
		return;
	}

	_realMousePos = { xpos, ypos };
	_mousePosX = xpos / _Height * 2 - _screenRatio;
	_mousePosY = (_Height - ypos) / _Height * 2 - 1;
}
void keyPressCallback(GLFWwindow* window, int button, int scancode, int action, int mods) {
	if (action == GLFW_PRESS)
		giveKeyAction::keyPressed(button);
	else if (action == GLFW_RELEASE)
		giveKeyAction::keyReleased(button);
	giveKeyAction::setNumlock(mods & GLFW_MOD_NUM_LOCK);
	giveKeyAction::setShift(mods & GLFW_MOD_SHIFT);
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
void addObjScript(Object*& obj, void* script) {
	((ObjectBase*)obj)->scripts.push_back(script);
	scriptBase* realScr = (scriptBase*)script;
	realScr->thisObj = obj;
	realScr->start();
}

/// <summary>
/// Remove an object script at a specific index
/// </summary>
void removeObjScript(Object*& obj, unsigned int index) {
	((scriptBase*)((ObjectBase*)obj)->scripts[index])->end();
	((scriptBase*)((ObjectBase*)obj)->scripts[index])->scrName.~basic_string();
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
		if (realScr->scrName == name) {
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
		((scriptBase*)((ObjectBase*)obj)->scripts[i])->scrName.~basic_string();
		delete(((ObjectBase*)obj)->scripts[i]);
	}
	((ObjectBase*)obj)->scripts.clear();
}

void clearObjChildren(Object*& obj) {
	if (obj->children.size() == 0)
		return;
	for (auto child : obj->children) {
        deleteObj(child);
	}
	obj->children.clear();
}

void updateObjChildren(Object*& obj) {
	if (obj->children.size() == 0)
		return;
	for (Object* child : obj->children) {
		child->setToRelative();
		updateObjChildren(child);
	}
}

bool objCmp(const ObjectBase* obj1, const ObjectBase* obj2) {
	if (obj1 == nullptr && obj2 == nullptr)
		return false;
	if (obj1 != nullptr && obj2 == nullptr)
		return true;
	if (obj1 == nullptr && obj2 != nullptr)
		return false;
    if (obj1->depth == obj2->depth)
        return false;
	return obj1->depth < obj2->depth;
}