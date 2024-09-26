#define _PROGRAM_CONTROL
#include "programControl.h"
#include "Windows.h"


uint _Width = 1280;
uint _Height = 720;
float _screenRatio = (float)_Width / _Height;
double mousePosX = 0;
double mousePosY = 0;

int main() {
	if (!glfwInit()) {
		std::cout << "Failed to load glfw\n";
		return 0;
	}

	GLFWwindow* mainWindow = glfwCreateWindow(_Width, _Height, "My window", nullptr, nullptr);
	if (!mainWindow) {
		std::cout << "Failed to load window\n";
		return 0;
	}
	glfwMakeContextCurrent(mainWindow);
	glfwSetWindowSizeCallback(mainWindow, windowScaled);
	glfwSetCursorPosCallback(mainWindow, mouseMove);
	glfwSetMouseButtonCallback(mainWindow, mouseClick);
	glfwSetKeyCallback(mainWindow, keyPress);
	glfwSetScrollCallback(mainWindow, mouseScroll);

	if (!gladLoadGL()) {
		std::cout << "Failed to load GL\n";
		return 0;
	}

	start();

	while (!glfwWindowShouldClose(mainWindow)) 
	{
		glfwPollEvents();
		update();
		glfwSwapBuffers(mainWindow);

	}

	end();

	return 1;
}



//Windows only
static int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR pCmdLine, _In_ int nCmdShow) {
	main();
}