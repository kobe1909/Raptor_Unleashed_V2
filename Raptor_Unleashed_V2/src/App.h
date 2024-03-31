#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include <iostream>
#include <functional>

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

public:
	glm::vec2 windowSize;
	glm::mat4 proj;

	bool CreateWindow(glm::vec2 windowSize, const char* title);

	void Run(std::function<void(double)> fun);
};

