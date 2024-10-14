#define _PROGRAM_CONTROL
#include "ProgramControl.h"
#include "Windows.h"


uint _Width = 1280;
uint _Height = 720;
float _screenRatio = (float)_Width / _Height;
double _deltaTime = 0;
bool _closeApp = false;

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
	glfwSetWindowSizeCallback(mainWindow, windowScaleCallback);
	glfwSetCursorPosCallback(mainWindow, mouseMoveCallback);
	glfwSetMouseButtonCallback(mainWindow, mouseClickCallback);
	glfwSetKeyCallback(mainWindow, keyPressCallback);
	glfwSetScrollCallback(mainWindow, mouseScrollCallback);
	glfwSetWindowPosCallback(mainWindow, windowMoveCallback);

	glfwSwapInterval(0);

	glfwSetInputMode(mainWindow, GLFW_LOCK_KEY_MODS, GLFW_TRUE);

	if (!gladLoadGL()) {
		std::cout << "Failed to load GL\n";
		return 0;
	}

	start();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	uint frameCount = 0;
	uint frameSay = 100;
	size_t fpsTotal = 0;
	size_t frameCatchCount = 0;


	while (!glfwWindowShouldClose(mainWindow))
	{
		std::chrono::time_point b = std::chrono::high_resolution_clock().now();
		glfwPollEvents();
		update();
		glfwSwapBuffers(mainWindow);

		frameCount++;

		if (_hideMouse)
			glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else
			glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		
		_deltaTime = (double)std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock().now() - b).count() / 1000000;
		if (frameCount >= frameSay) {
			size_t fps = 1000000 / std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock().now() - b).count();
			fpsTotal += fps;
			frameCatchCount++;
			frameCount = 0;
		}
	}

	std::cout << "\n\n\nAverage fps : " << fpsTotal / frameCatchCount << "\n";

	end();

	return 1;
}



//Windows only
static int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR pCmdLine, _In_ int nCmdShow) {
	main();
}