#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"
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
	bool CreateWindow(int width, int height, const char* title);

	void Run(std::function<void(double)> fun);
};

