#pragma once

#include "GLFW/glfw3.h"
#include <functional>

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

