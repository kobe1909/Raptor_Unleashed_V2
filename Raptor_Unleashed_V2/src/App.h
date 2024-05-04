#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include <iostream>
#include <vector>
#include <functional>
#include "Scene.h"

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCALL(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__));

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);

class App {
private:
	GLFWwindow* window;
	double lastTime = 0;
	int* buffer = nullptr;

public:
	glm::vec2 windowSize;
	glm::mat4 proj;

	glm::vec2 mousePos;
	glm::vec2 lastMousePos;
	glm::vec2 mouseOffset;

	std::vector<Scene*> scenes;
	int activeScene = -1;

	bool firstMouseMove = true;

	bool GetKeyState(int key, int state);
	bool GetKeyState(int key);

	void mouseMoveCallBack(GLFWwindow* window, double xpos, double ypos) {
		mousePos = glm::vec2(xpos, ypos);
	}

	bool CreateWindow(glm::vec2 windowSize, const char* title, bool maximize);

	void Run(std::function<void(double)> fun);
	
	void CleanUp();
};

