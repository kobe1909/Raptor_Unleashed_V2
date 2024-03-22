#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <functional>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCALL(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__));

void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		std::cout << "[OpenGL Error] (" << error << "): " << function << " in " << file << " on " << line << std::endl;
		return false;
	}

	return true;
}

class App {
private:
	GLFWwindow* window;

public:
	bool CreateWindow(int width, int height, const char *title) {
		if (!glfwInit())
			return false;
		 
		window = glfwCreateWindow(width, height, title, NULL, NULL);
		if (!window) {
			glfwTerminate();
			return false;
		}

		glfwMakeContextCurrent(window);
		glClearColor(1, 0, 0, 1);

		return true;
	}

	void Run(std::function<void()> fun) {
		while (!glfwWindowShouldClose(window)) {
			glClear(GL_COLOR_BUFFER_BIT);

			fun();

			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}
};

